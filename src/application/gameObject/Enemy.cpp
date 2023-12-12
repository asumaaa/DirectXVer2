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
	Sprite* newSprite = new Sprite();
	newSprite->SetTextureNum(4);
	newSprite->Initialize();
	spriteHpBar.reset(newSprite);

	//立っているモデル
	modelStand = FbxLoader::GetInstance()->LoadModelFromFile("enemyStand");
	//立っているオブジェクト
	objectStand = new FbxObject3D;
	objectStand->Initialize();
	objectStand->SetModel(modelStand);
	objectStand->PlayAnimation();

	//歩いているモデル
	modelWalk = FbxLoader::GetInstance()->LoadModelFromFile("enemyWalk");
	//歩いているオブジェクト
	objectWalk = new FbxObject3D;
	objectWalk->Initialize();
	objectWalk->SetModel(modelWalk);
	objectWalk->StopAnimation();

	//攻撃1のモデル
	modelAttack1 = FbxLoader::GetInstance()->LoadModelFromFile("enemyAttack1");
	//攻撃1のオブジェクト
	objectAttack1 = new FbxObject3D;
	objectAttack1->Initialize();
	objectAttack1->SetModel(modelAttack1);
	objectAttack1->StopAnimation();

	//攻撃前兆のモデル
	modelAttack1Omen = FbxLoader::GetInstance()->LoadModelFromFile("enemyAttack1Omen");
	//攻撃前兆のオブジェクト
	objectAttack1Omen = new FbxObject3D;
	objectAttack1Omen->Initialize();
	objectAttack1Omen->SetModel(modelAttack1Omen);
	objectAttack1Omen->StopAnimation();

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

	//1フレーム前の状態を代入
	preStatus = status;
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
	//HPバー
	spriteHpBar->SetAlpha(1.0f);
	spriteHpBar->SetScale({ 100.0f, 100.0 });
	spriteHpBar->SetPosition({ 0.0f, 0.0 });
	spriteHpBar->Update();
}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
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
	spriteHpBar->Draw(cmdList);
}

void Enemy::DrawParticle(ID3D12GraphicsCommandList* cmdList)
{
	bullet->DrawParticle(cmdList);
}

void Enemy::Move()
{
	////ジャンプ更新
	//UpdateJump();

	////重力更新
	//UpdateGravity();

	//position.x -= input->PushKey(DIK_A) * speed;
	//position.x += input->PushKey(DIK_D) * speed;
	//position.z -= input->PushKey(DIK_S) * speed;
	//position.z += input->PushKey(DIK_W) * speed;
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

void Enemy::StatusManager()
{
	//攻撃前兆の場合
	if (status == Attack1Omen)
	{
		//弾をセット
		if (statusTimer == 0.0f)
		{
			for (int i = 0; i < bulletVol; i++)
			{
				bullet->SetBullet(position, bulletScale, bulletLastScale, 0.0f,
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
		if (statusTimer >= frameWalk)
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