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

	//モデルの削除
	delete modelWait;
	delete modelRun;
	delete modelAttack1;
	delete modelAttack2;
}

void Player::Initialize()
{
	//待ってるモデル
	modelWait = FbxLoader::GetInstance()->LoadModelFromFile("playerWait");
	//待っているオブジェクト
	objectWait = new FbxObject3D;
	objectWait->Initialize();
	objectWait->SetModel(modelWait);
	objectWait->PlayAnimation();

	//走ってるモデル
	modelRun = FbxLoader::GetInstance()->LoadModelFromFile("playerRun");
	//走ってるオブジェクト
	objectRun = new FbxObject3D;
	objectRun->Initialize();
	objectRun->SetModel(modelRun);
	objectRun->StopAnimation();

	//攻撃1のモデル
	modelAttack1 = FbxLoader::GetInstance()->LoadModelFromFile("playerAttack1");
	//攻撃1のオブジェクト
	objectAttack1 = new FbxObject3D;
	objectAttack1->Initialize();
	objectAttack1->SetModel(modelAttack1);
	objectAttack1->StopAnimation();

	//攻撃2のモデル
	modelAttack2 = FbxLoader::GetInstance()->LoadModelFromFile("playerAttack2");
	//攻撃2のオブジェクト
	objectAttack2 = new FbxObject3D;
	objectAttack2->Initialize();
	objectAttack2->SetModel(modelAttack2);
	objectAttack2->StopAnimation();

	//雷パーティクル
	ThunderParticle* newThunderParticle = new ThunderParticle();
	newThunderParticle->CreateBuffers();
	thunderParticle.reset(newThunderParticle);

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
	rockOnSprite->SetPosition(rockOnSpritePos);
	rockOnSprite->SetScale(rockOnSpriteScale);
}

void Player::Update()
{
	//弾更新
	UpdateParticle();

	//挙動
	GameControl();

	//コライダー設定
	UpdateCollider();

	//攻撃
	UpdateAttack();

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
}

void Player::UpdateTitle(float timer)
{
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

	//弾更新
	bullet1->SetEnemyPos(enemyPos);
	bullet1->Update();
}

void Player::UpdateObject(Status status, FbxObject3D* object)
{
	if (this->status == status)
	{
		object->SetPosition(position);
		object->SetRotation(rotation0 + rotation1);
		object->SetScale(scale);
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
	if (Attack1Timer == 1)
	{
		thunderParticle->Add(XMFLOAT3(-15.0f, 15.0f, 0.0f), XMFLOAT3(10.0f, 15.0f, 90.0f));
	}
	thunderParticle->Update();
}

void Player::UpdateSprite()
{
	rockOnSprite->Update();
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

	//弾
	bullet1->Draw();
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
	rockOnSprite->Draw(cmdList);
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

void Player::GameMove()
{
	//Attack1,Attack2中は移動できない
	if (status == Attack1 || status == Attack2)return;

	//プレイヤーの元になる角度
	//AROWキーで角度変更
	rotVelocity.y = dxInput->GetStick(DXInput::RStickX) * rot0Speed;
	/*rotVelocity.y = (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * rotSpeed;*/
	//角度ベクトルを加算
	rotation0 = rotation0 + rotVelocity;

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
	//インターバル中にBボタンで連続攻撃 攻撃2へ
	if (dxInput->TriggerKey(DXInput::PAD_B) && 0 <= Attack1Time - Attack1Timer && Attack1Time - Attack1Timer <= Attack1IntervalTime)
	{
		status = Attack2;
		return;
	}
	//攻撃モーション中 攻撃2
	else if (Attack2Timer >= 1)
	{
		status = Attack2;
		return;
	}
	//Bボタンで攻撃1
	else if (dxInput->TriggerKey(DXInput::PAD_B) && Attack1Timer == 0 || Attack1Timer >= 1)
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

void Player::UpdateBullet1()
{
	//Aボタンを押している間
	if (dxInput->PushKey(DXInput::PAD_RIGHT_SHOULDER))
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