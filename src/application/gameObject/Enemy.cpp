/**
 * @file Enemy.cpp
 * @brief ゲームオブジェクト 敵
 * @author Asuma Syota
 * @date 2023/4
 */

#include "Enemy.h"
#include "mathOriginal.h"
#include "FbxLoader.h"
#include "imgui.h"
#include "ColliderManager.h"
#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

Camera* Enemy::camera = nullptr;
Input* Enemy::input = nullptr;
DXInput* Enemy::dxInput = nullptr;

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	//HPバーのスプライト
	hpBar1 = new Sprite();
	hpBar1->Initialize();
	hpBar1->SetTextureNum(23);
	hpBar2 = new Sprite();
	hpBar2->Initialize();
	hpBar2->SetTextureNum(24);
	hpBar3 = new Sprite();
	hpBar3->Initialize();
	hpBar3->SetTextureNum(25);
	hpBar4 = new Sprite();
	hpBar4->Initialize();
	hpBar4->SetTextureNum(26);

	//立っているモデル
	modelStand = FbxLoader::GetInstance()->LoadModelFromFile("enemyStand");
	//立っているオブジェクト
	objectStand = new FbxObject3D;
	objectStand->Initialize();
	objectStand->SetModel(modelStand);
	objectStand->SetTextureNum(0);
	objectStand->PlayAnimation();

	//歩いているモデル
	modelWalk = FbxLoader::GetInstance()->LoadModelFromFile("enemyWalk");
	//歩いているオブジェクト
	objectWalk = new FbxObject3D;
	objectWalk->Initialize();
	objectWalk->SetModel(modelWalk);
	objectWalk->SetTextureNum(0);
	objectWalk->StopAnimation();

	//攻撃1のモデル
	modelAttack1 = FbxLoader::GetInstance()->LoadModelFromFile("enemyAttack1");
	//攻撃1のオブジェクト
	objectAttack1 = new FbxObject3D;
	objectAttack1->Initialize();
	objectAttack1->SetModel(modelAttack1);
	objectAttack1->SetTextureNum(0);
	objectAttack1->StopAnimation();

	//攻撃前兆のモデル
	modelAttack1Omen = FbxLoader::GetInstance()->LoadModelFromFile("enemyAttack1Omen");
	//攻撃前兆のオブジェクト
	objectAttack1Omen = new FbxObject3D;
	objectAttack1Omen->Initialize();
	objectAttack1Omen->SetModel(modelAttack1Omen);
	objectAttack1Omen->SetTextureNum(0);
	objectAttack1Omen->StopAnimation();

	//コライダーの設定
	colliderData.type = "Sphere";	//判定を球体で取るため
	colliderData.objectName = "enemy";
	colliderData.scale = scale;
	colliderData.rotation = rotation;
	colliderData.center = position;
	//コライダーマネージャーにセット
	ColliderManager::SetCollider(colliderData);

	//敵の弾
	bullet = new EnemyBullet;
	bullet->Initialize();
}

void Enemy::Update()
{
	//動く
	Move();

	//ステータスマネージャー
	StatusManager();

	//オブジェクト更新
	UpdateObject();

	//スプライト更新
	UpdateSprite();

	//ダメージ更新
	UpdateDamage();

	//コライダー更新
	UpdateCollider();

	//1フレーム前の状態を代入
	preStatus = status;

	//デバッグ用に死なないようにする
	if (HP <= 0)HP = maxHP;
}

void Enemy::UpdateObject()
{
	//オブジェクト更新
	UpdateObject(Stand, objectStand);
	UpdateObject(Walk, objectWalk);
	UpdateObject(Attack1, objectAttack1);
	UpdateObject(Attack1Omen, objectAttack1Omen);

	//弾更新
	bullet->SetPlayerPos(playerPos);
	bullet->Update();
}

void Enemy::UpdateObject(Status status, FbxObject3D* object)
{
	//引数のステータスと同じ場合のみ更新
	if (this->status == status)
	{
		object->SetPosition(position);
		object->SetRotation(rotation);
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

void Enemy::UpdateSprite()
{
	//HPバーを現在のHPに
	hpBar2Scale.x = hpBar2OriginalScale.x * (HP / maxHP);
	hpBar3Pos.x = hpBar3OriginalPos.x - (hpBar2OriginalScale.x * ((maxHP - HP) / maxHP));

	//更新
	hpBar1->Update(hpBar1Pos, hpBar1Scale);
	hpBar2->Update(hpBar2Pos, hpBar2Scale);
	hpBar3->Update(hpBar3Pos, hpBar3Scale);
	hpBar4->Update(hpBar4Pos, hpBar4Scale);
}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//ImGui
	/*ImGui::Begin("Enemy");
	ImGui::SetWindowPos(ImVec2(0, 150));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputFloat2("HP", pos1);
	ImGui::End();*/

	if (status == Stand)
	{
		objectStand->Draw(cmdList);
	}
	if (status == Walk)
	{
		objectWalk->Draw(cmdList);
	}
	if (status == Attack1)
	{
		objectAttack1->Draw(cmdList);
	}
	if (status == Attack1Omen)
	{
		objectAttack1Omen->Draw(cmdList);
	}

	//弾
	bullet->Draw();

	//ImGui
	/*ImGui::Begin("Enemy");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputFloat3("playerPos", bulletPos);
	ImGui::End();*/
}

void Enemy::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	if (status == Stand)
	{
		objectStand->DrawLightView(cmdList);
	}
	if (status == Walk)
	{
		objectWalk->DrawLightView(cmdList);
	}
	if (status == Attack1)
	{
		objectAttack1->DrawLightView(cmdList);
	}
	if (status == Attack1Omen)
	{
		objectAttack1Omen->DrawLightView(cmdList);
	}
}

void Enemy::DrawSprite(ID3D12GraphicsCommandList* cmdList)
{
	hpBar2->Draw(cmdList);
	hpBar4->Draw(cmdList);
	hpBar1->Draw(cmdList);
	hpBar3->Draw(cmdList);
}

void Enemy::DrawParticle(ID3D12GraphicsCommandList* cmdList)
{
	bullet->DrawParticle(cmdList);
}

void Enemy::Move()
{
	//歩きモーション時の動き
	if (status == Walk)
	{
		MoveWalk();
	}
}

void Enemy::MoveWalk()
{
	//プレイヤーと敵のベクトル取得
	XMFLOAT3 velo = playerPos - position;
	//敵がプレイヤーの向きを向くようにする
	rotation.y = getVectorRotation(velo).y;

	//プレイヤーに近づく
	position = position + normalize(velo) * walkSpeed;
}

void Enemy::UpdateGravity()
{
	//接地していたらタイマーとベクトルリセット
	if (groundFlag == true)
	{
		fallTimer = 0.0f;
		fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	//接地していなければ
	if (groundFlag == false)
	{
		//落下タイマーが最大値より小さければ
		if (fallTimer < fallTime)
		{
			fallTimer += fallFrame;
		}
	}

	//落下ベクトル計算
	fallVelocity.y = -(GAcceleration * fallTimer);

	//座標に落下ベクトルを加算
	position.x += fallVelocity.x;
	position.y += fallVelocity.y;
	position.z += fallVelocity.z;
}

void Enemy::UpdateJump()
{
	//接地していたら
	if (groundFlag == true)
	{
		//スペースキーでジャンプ
		if (input->TriggerKey(DIK_SPACE))
		{
			groundFlag = false;
			fallTimer = -jumpHeight;
			fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}
}

void Enemy::UpdateAttack()
{
	
}

void Enemy::UpdateAttack1()
{
}

void Enemy::UpdateAttackOmen()
{
}

void Enemy::UpdateDamage()
{
	//炎攻撃をくらった際
	if (HitFlag1 == true)
	{
		//HPを減らす
		HP -= 1.0f;
	}

	//死亡
	if (HP <= 0)
	{
		isDead = true;
	}

	//フラグをもとに戻す
	HitFlag1 = false;
}

void Enemy::UpdateCollider()
{
	colliderData.scale = XMFLOAT3(20.0f,10.0f,10.0f);
	colliderData.rotation = rotation;
	colliderData.center = position + XMFLOAT3(0.0f,5.0f,0.0f);
}

void Enemy::StatusManager()
{
	//攻撃前兆の場合
	if (status == Attack1Omen)
	{
		//弾をセット
		if (statusTimer == 0.0f)
		{
			XMFLOAT3 addPos(-20, 40, 50);
			addPos = position + rollRotation(addPos,rotation);
			for (int i = 0; i < bulletVol; i++)
			{
				bullet->SetBullet(addPos, bulletScale, bulletLastScale, 0.0f,
					frameAttack1Omen + bulletTimeLag * i, frameAttack1);
			}
		}
		//タイマー更新
		statusTimer += 1.0f;
		if (statusTimer >= frameAttack1Omen)
		{
			statusTimer = 0.0f;
			status = Attack1;
			return;
		}
	}
	//攻撃1の場合
	if (status == Attack1)
	{
		//タイマー更新
		statusTimer += 1.0f;
		if (statusTimer >= frameAttack1)
		{
			statusTimer = 0.0f;
			status = Stand;
			return;
		}
	}
	//立っているの場合
	if (status == Stand)
	{
		//タイマー更新
		statusTimer += 1.0f;
		if (statusTimer >= frameStand)
		{
			statusTimer = 0.0f;
			status = Walk;
			return;
		}
	}
	//歩きの場合
	if (status == Walk)
	{
		//タイマー更新
		statusTimer += 1.0f;
		//プレイヤーとの距離が近い場合も終了
		if (statusTimer >= frameWalk || length(position - playerPos) <= 10.0f)
		{
			statusTimer = 0.0f;
			status = Attack1Omen;
			return;
		}
	}
}

void Enemy::SetSRV(ID3D12DescriptorHeap* SRV)
{
	if (status == Stand)
	{
		objectStand->SetSRV(SRV);
	}
	if (status == Walk)
	{
		objectWalk->SetSRV(SRV);
	}
	if (status == Attack1)
	{
		objectAttack1->SetSRV(SRV);
	}
	if (status == Attack1Omen)
	{
		objectAttack1Omen->SetSRV(SRV);
	}
}

void Enemy::HitPlane()
{
	//接地フラグを立てる
	groundFlag = true;

	//めり込まなくなるまで加算
	position.y += 0.1f;

	//オブジェクト更新
	UpdateObject();
}

void Enemy::HitBullet1()
{
	//被弾フラグを立てる
	HitFlag1 = true;
}

void Enemy::Reset()
{
	HP = maxHP;
	isDead = false;
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
