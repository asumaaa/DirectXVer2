/**
 * @file Player.cpp
 * @brief ゲームオブジェクト プレイヤー
 * @author Asuma Syota
 * @date 2023/4
 */

#include "Player.h"
#include "mathOriginal.h"
#include "FbxLoader.h"
#include "ColliderManager.h"
#include "imgui.h"
#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

Camera* Player::camera = nullptr;
Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

Player::~Player()
{
	//オブジェクトの削除
	delete objectWait;
	delete objectRun;
	delete objectAttack1;
	delete objectAttack2;
	delete objectAttack3;
	delete objectDown;

	//モデルの削除
	delete modelWait;
	delete modelRun;
	delete modelAttack1;
	delete modelAttack2;
	delete modelAttack3;
	delete modelDown;
}

void Player::Initialize()
{
	//プレイヤーのシェーダを別に設定
	JSONLoader::TextureData textureData;
	textureData.textureVol = 2;
	textureData.shaderVol = 2;
	textureData.textureNum1 = 27;	//白いテクスチャ
	textureData.textureNum2 = 31;	//炎のテクスチャ
	textureData.shaderName = "PlayerFire";	//シェーダの名前
	textureData.shaderName2 = "PlayerElec";	//シェーダの名前
	/*newObject->SetTextureData(jsonLoader->GetTextureData(i));*/

	//待ってるモデル
	modelWait = FbxLoader::GetInstance()->LoadModelFromFile("playerWait");
	//待っているオブジェクト
	objectWait = new FbxObject3D;
	objectWait->Initialize(modelWait,textureData);

	//走ってるモデル
	modelRun = FbxLoader::GetInstance()->LoadModelFromFile("playerRun");
	//走ってるオブジェクト
	objectRun = new FbxObject3D; 
	objectRun->Initialize(modelRun, textureData);

	//攻撃1のモデル
	modelAttack1 = FbxLoader::GetInstance()->LoadModelFromFile("playerAttack1");
	//攻撃1のオブジェクト
	objectAttack1 = new FbxObject3D;
	objectAttack1->Initialize(modelAttack1, textureData);

	//攻撃2のモデル
	modelAttack2 = FbxLoader::GetInstance()->LoadModelFromFile("playerAttack2");
	//攻撃2のオブジェクト
	objectAttack2 = new FbxObject3D;
	objectAttack2->Initialize(modelAttack2, textureData);

	//攻撃2のモデル
	modelAttack3 = FbxLoader::GetInstance()->LoadModelFromFile("playerRun");
	//攻撃2のオブジェクト
	objectAttack3 = new FbxObject3D;
	objectAttack3->Initialize(modelAttack3, textureData);

	//被ダメージ時のモデル
	modelDown = FbxLoader::GetInstance()->LoadModelFromFile("playerDown");
	//被ダメージ時のオブジェクト
	objectDown = new FbxObject3D;
	objectDown->Initialize(modelDown, textureData);

	//コライダーの設定
	colliderData.type = "Sphere";	//判定を球体で取るため
	colliderData.objectName = "player";
	colliderData.scale = scale;
	colliderData.rotation = rotation0;
	colliderData.center = position;
	//コライダーマネージャーにセット
	ColliderManager::SetCollider(colliderData);

	//弾
	bullet1 = new PlayerBullet;
	bullet1->Initialize();

	//タイトルのスプライト
	rockOnSprite = new Sprite();
	rockOnSprite->Initialize();
	rockOnSprite->SetTextureNum(22);

	//HP用スプライト
	//緑のHP
	for (int i = 0; i < MaxHP; i++)
	{
		hpSprite1[i] = new Sprite();
		hpSprite1[i]->Initialize();
		hpSprite1[i]->SetTextureNum(28);
	}
	//赤いHP
	hpSprite2 = new Sprite();
	hpSprite2->Initialize();
	hpSprite2->SetTextureNum(29);
	//HPの枠
	hpSprite3 = new Sprite();
	hpSprite3->Initialize();
	hpSprite3->SetTextureNum(30);
}
void Player::Update()
{
	//フォルム更新
	UpdateForm();

	//弾更新
	UpdateParticle();

	//挙動
	GameControl();

	//コライダー設定
	UpdateCollider();

	//攻撃
	UpdateAttack();

	//被ダメージ時
	UpdateDown();

	//ステータスマネージャー
	StatusManager();

	//炎の弾更新
	UpdateBullet1();

	//オブジェクト更新
	UpdateObject();

	//スプライト更新
	UpdateSprite();

	//1フレーム前の状態を代入
	preStatus = status;

	//1フレーム前のフォルムを代入
	preForm = form;

	//当たりフラグを元に戻す
	hitFlag = false;

	if (HP <= 0)HP = MaxHP;
}

void Player::UpdateTitle(float timer)
{
	//フォルム更新
	UpdateForm();

	//弾更新
	UpdateParticle();

	//挙動
	TitleControl(timer);

	//コライダー設定
	UpdateCollider();

	//攻撃
	UpdateAttack();

	//ステータスマネージャー
	StatusManager();

	//オブジェクト更新
	UpdateObject();

	//1フレーム前の状態を代入
	preStatus = status;

	//1フレーム前のフォルムを代入
	preForm = form;

	//1フレーム前のトランスフォームを保存
	UpdateOldTransform();
}

void Player::UpdateCollider()
{
	colliderData.scale = scale;
	colliderData.rotation = rotation0;
	colliderData.center = position;
}

void Player::UpdateObject()
{
	UpdateObject(Wait, objectWait);
	UpdateObject(Run, objectRun);
	UpdateObject(Attack1, objectAttack1);
	UpdateObject(Attack2, objectAttack2);
	UpdateObject(Attack3, objectAttack3);
	UpdateObject(Down, objectDown);

	//弾更新
	bullet1->SetEnemyPos(enemyPos);
	bullet1->Update();
}

void Player::UpdateObject(const Status &status, FbxObject3D* object)
{
	if (this->status == status)
	{
		object->SetPosition(position);
		object->SetRotation(rotation0 + rotation1);
		object->SetScale(scale);
		object->SetDrawShaderNum(form);
		if (this->status != preStatus)
		{
			object->PlayAnimation();
		}

		object->Update();
	}
	else
	{
		object->StopAnimation();
	}
}

void Player::UpdateParticle()
{
}

void Player::UpdateSprite()
{
	rockOnSprite->Update(rockOnSpritePos, rockOnSpriteScale);

	//緑のHP
	for (int i = 0; i < HP; i++)
	{
		XMFLOAT2 addPos(hpFrameScale1 + (hpFrameScale2 * i) + (hpSprite1Scale.x * i), hpFrameScale3);
		hpSprite1[i]->Update(hpSpritePos + addPos, hpSprite1Scale);
	}
	//赤いHP
	hpSprite2->Update(hpSpritePos + XMFLOAT2(hpFrameScale1, hpFrameScale3), hpSprite2Scale);
	//HPの枠
	hpSprite3->Update(hpSpritePos, hpSprite3Scale);
}

void Player::UpdateOldTransform()
{
	oldPosition = position;
	oldRotation0 = rotation0;
	oldRotation1 = rotation1;
	oldPosVelocity = posVelocity;
	oldRotVelocity = rotVelocity;
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (status == Wait)
	{
		objectWait->Draw(cmdList);
	}
	if (status == Run)
	{
		objectRun->Draw(cmdList);
	}
	if (status == Attack1)
	{
		objectAttack1->Draw(cmdList);
	}
	if (status == Attack2)
	{
		objectAttack2->Draw(cmdList);
	}
	if (status == Attack3)
	{
		objectAttack3->Draw(cmdList);
	}
	if (status == Down)
	{
		objectDown->Draw(cmdList);
	}

	//弾
	bullet1->Draw();

	//ImGui
	/*ImGui::Begin("Player");
	ImGui::SetWindowPos(ImVec2(0, 300));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputFloat2("debugPos", debugPos);
	ImGui::End();*/
	
}

void Player::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	if (status == Wait)
	{
		objectWait->DrawLightView(cmdList);
	}
	if (status == Run)
	{
		objectRun->DrawLightView(cmdList);
	}
	if (status == Attack1)
	{
		objectAttack1->DrawLightView(cmdList);
	}
	if (status == Attack2)
	{
		objectAttack2->DrawLightView(cmdList);
	}
	if (status == Attack3)
	{
		objectAttack3->DrawLightView(cmdList);
	}
	if (status == Down)
	{
		objectDown->DrawLightView(cmdList);
	}
}

void Player::DrawParticle(ID3D12GraphicsCommandList* cmdList)
{
	//雷描画
	/*thunderParticle->Draw(cmdList);*/
	//弾
	bullet1->DrawParticle(cmdList);
}

void Player::DrawSprite(ID3D12GraphicsCommandList* cmdList)
{
	/*rockOnSprite->Draw(cmdList);*/

	//緑のHP
	for (int i = 0; i < HP; i++)
	{
		hpSprite1[i]->Draw(cmdList);
	}
	//赤いHP
	if (HP == 1)
	{
		hpSprite2->Draw(cmdList);
	}
	//HPの枠
	hpSprite3->Draw(cmdList);
}

void Player::TitleControl(float timer)
{
	//ジャンプ更新
	UpdateJump();

	//重力更新
	UpdateGravity();

	//移動
	TitleMove(timer);
}

void Player::GameControl()
{
	//ジャンプ更新
	UpdateJump();

	//重力更新
	UpdateGravity();

	//移動
	GameMove();
}

void Player::UpdateForm()
{
	//Lボタンでフォルムチェンジフラグを立てる
	if (dxInput->TriggerKey(DXInput::PAD_LEFT_SHOULDER) && formChangeFlag == false)
	{
		if (preForm == Fire)form = Elec;	//炎だったら電気に変える
		else if (preForm == Elec)form = Fire;	//電気だったら炎に帰る
		formChangeFlag = true;
	}
	//フォルムチェンジフラグがたったら
	if (formChangeFlag == true)
	{
		//タイマー更新
		formTimer++;
		//タイマー最大まで経ったら
		if (formTimer >= formMaxTime)
		{
			formChangeFlag = false;
			formTimer = 0;
		}
	}
}

void Player::GameMove()
{
	//プレイヤーの元になる角度
	//AROWキーで角度変更
	rotVelocity.y = dxInput->GetStick(DXInput::RStickX) * rot0Speed;
	/*rotVelocity.y = (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * rotSpeed;*/
	//角度ベクトルを加算
	rotation0 = rotation0 + rotVelocity;

	//Attack1,Attack2,被弾中は移動できない
	if (status == Attack1 || status == Attack2 || status == Down)return;

	//左スティック入力による角度変更
	//左スティックの入力がある場合
	if (dxInput->GetStick(DXInput::LStick) != 0.0f)
	{
		rotation1.y = dxInput->GetStickRot(DXInput::LStick);
	}
	//左スティックの入力がないけど右スティックの入力はある場合
	else if(dxInput->GetStick(DXInput::RStick) && dxInput->GetStickRot(DXInput::RStick) <= PI)
	{
		rotation1 = rotation1 - rotVelocity;
	}
	else if (dxInput->GetStick(DXInput::RStick))
	{
		rotation1 = rotation1 + rotVelocity;
	}

	//座標
	//左スティックで移動
	XMFLOAT2 stick = normalize(dxInput->GetStick(DXInput::LStickX), dxInput->GetStick(DXInput::LStickY));
	posVelocity.x = stick.x;
	posVelocity.z = stick.y;
	/*posVelocity.x = (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * posSpeed;
	posVelocity.z = (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * posSpeed;*/
	//進行ベクトルを回転
	posVelocity = rollRotation(posVelocity, rotation0);
	//進行ベクトルを加算
	position = position + posVelocity;
}

void Player::TitleMove(float timer)
{
	//通常のタイトルシーン
	if (timer == 0)
	{
		//AROWキーで角度変更
		rotVelocity.y = 0.01f;
		//角度ベクトルを加算
		rotation0 = rotation0 + rotVelocity;

		//ASDWで移動
		posVelocity.x = 0.9;
		posVelocity.z = -0.1;
	}

	//シーン遷移タイマーが始動したら
	if (timer > 0)
	{
		if (timer == 1)
		{
			//走るアニメーションのフラグを立てる
			runAnimationFlag = true;
			//最初に加算する角度を取得
			addRot = rotation0.y - (float)PI / 2.0f;
			originalRot = rotation0.y;
			//座標を固定
			posVelocity.x = 0.0;
			posVelocity.z = 0.0;
		}
		//角度加算 プレイヤーが背を向けるように
		if (timer < 120)
		{
			rotation0.y = (originalRot - addRot) * easeOutCirc(timer / 120.0f);
		}
		if (timer > 120)
		{
			posVelocity.x = 0.0;
			posVelocity.z += 3.0;
		}
		if (timer == 299)
		{
			runAnimationFlag = false;
		}
	}

	//進行ベクトルを回転
	posVelocity = rollRotation(posVelocity, rotation0);
	//進行ベクトルを加算
	position = position + posVelocity;
}

void Player::UpdateGravity()
{
	////接地していたらタイマーとベクトルリセット
	//if (groundFlag == true)
	//{
	//	fallTimer = 0.0f;
	//	fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//}

	////接地していなければ
	//if (groundFlag == false)
	//{
	//	//落下タイマーが最大値より小さければ
	//	if (fallTimer < fallTime)
	//	{
	//		fallTimer += fallFrame;
	//	}
	//}

	////落下ベクトル計算
	//fallVelocity.y = -(GAcceleration * fallTimer);

	////座標に落下ベクトルを加算
	//position = position + fallVelocity;
}

void Player::UpdateJump()
{
	////接地していたら
	//if (groundFlag == true)
	//{
	//	//スペースキーでジャンプ
	//	if (input->TriggerKey(DIK_SPACE))
	//	{
	//		groundFlag = false;
	//		fallTimer = -jumpHeight;
	//		fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	}
	//}
}

void Player::StatusManager()
{
	//ヒットフラグが立っていたら	被ダメージモーションへ
	if (hitTimer <= frameDown && invincibleFlag == true)
	{
		status = Down;
		return;
	}

	//炎状態でRボタンを押していたら攻撃3へ
	if (dxInput->PushKey(DXInput::PAD_RIGHT_SHOULDER) == 1 && form == Fire)
	{
		status = Attack3;
		return;
	}

	//インターバル中にRボタンで連続攻撃 攻撃2へ
	if (form == Elec && dxInput->TriggerKey(DXInput::PAD_RIGHT_SHOULDER) && 0 <= Attack1Time - Attack1Timer && Attack1Time - Attack1Timer <= Attack1IntervalTime)
	{
		status = Attack2;
		return;
	}
	//攻撃モーション中 攻撃2
	else if (form == Elec && Attack2Timer >= 1)
	{
		status = Attack2;
		return;
	}
	//Rボタンで攻撃1
	else if (form == Elec && dxInput->TriggerKey(DXInput::PAD_RIGHT_SHOULDER) && Attack1Timer == 0 || Attack1Timer >= 1)
	{
		status = Attack1;
		return;
	}
	//走っているアニメーションフラグが立ってる場合
	else if (runAnimationFlag == true)
	{
		status = Run;
		return;
	}
	//立ち止まっている場合
	else if (length(posVelocity) <= 0.01f)
	{
		status = Wait;
		return;
	}
	//普通に走っている場合
	else
	{
		status = Run;
		return;
	}
}

void Player::UpdateAttack()
{
	//タイマーの更新
	if (status == Attack1)
	{
		Attack1Timer++;
	}
	//攻撃終了したら
	if (Attack1Timer >= Attack1Time)
	{
		//タイマー戻す
		Attack1Timer = 0.0f;
	}

	//タイマーの更新
	if (status == Attack2)
	{
		Attack2Timer++;
	}
	//攻撃終了したら
	if (Attack2Timer >= Attack2Time)
	{
		//タイマー戻す
		Attack2Timer = 0.0f;
		Attack1Timer = 0.0f;
	}
}

void Player::UpdateDown()
{
	//ヒットフラグがたったら
	if (hitFlag == true)
	{
		invincibleFlag = true;
	}

	//無敵時間が終わったらフラグを元に戻す
	if (hitTimer >= frameDown * 2.0f)
	{
		invincibleFlag = false;
		hitTimer = 0.0f;
	}

	//無敵時間中
	if (invincibleFlag == true)
	{
		//タイマー更新
		hitTimer++;
	}
}

void Player::UpdateBullet1()
{
	//Aボタンを押している間
	if (status == Attack3)
	{
		//タイマー更新
		bullet1Timer += 1.0f;

		//弾追加
		if (bullet1Timer % bullet1Interval == 0.0f)
		{
			XMFLOAT3 velo = rollRotation(XMFLOAT3(0.0f,0.0f,1.0f),rotation0);
			velo = normalize(velo);
			bullet1->SetBullet(position + bullet1AddPosition, velo, 0.0f, bullet1Frame);
		}

	}
}

void Player::SetSRV(ID3D12DescriptorHeap* SRV)
{
	if (status == Wait)
	{
		objectWait->SetSRV(SRV);
	}
	if (status == Run)
	{
		objectRun->SetSRV(SRV);
	}
	if (status == Attack1)
	{
		objectAttack1->SetSRV(SRV);
	}
	if (status == Attack2)
	{
		objectAttack2->SetSRV(SRV);
	}
	if (status == Attack3)
	{
		objectAttack3->SetSRV(SRV);
	}
	if (status == Down)
	{
		objectDown->SetSRV(SRV);
	}
}

void Player::HitPlane()
{
	//接地フラグを立てる
	groundFlag = true;

	//めり込まなくなるまで加算
	position.y += 0.1f;

	//オブジェクト更新
	UpdateObject();
}

void Player::HitEnemy()
{
	//当たりフラグを立てる
	hitFlag = true;

	//HPを減らす
	HP--;

	if (HP <= 0)
	{
		isDead = true;
	}
}

void Player::Reset()
{
	HP = MaxHP;
	position = XMFLOAT3(0.0f, 0.0f, -20.0f);
}
