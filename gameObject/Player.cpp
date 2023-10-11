#include "Player.h"
#include "mathOriginal.h"
#include "FbxLoader.h"
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
	delete objectBackRun;
	delete objectRunLeft;
	delete objectRunRight;
	delete objectAttack1;
	delete objectAttack2;

	//モデルの削除
	delete modelWait;
	delete modelRun;
	delete modelBackRun;
	delete modelRunLeft;
	delete modelRunRight;
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

	//後方に走るモデル
	modelBackRun = FbxLoader::GetInstance()->LoadModelFromFile("playerBackRun");
	//後方に走るオブジェクト
	objectBackRun = new FbxObject3D;
	objectBackRun->Initialize();
	objectBackRun->SetModel(modelBackRun);
	objectBackRun->StopAnimation();

	//左に走るモデル
	modelRunLeft = FbxLoader::GetInstance()->LoadModelFromFile("playerRunLeft");
	//左に走るオブジェクト
	objectRunLeft = new FbxObject3D;
	objectRunLeft->Initialize();
	objectRunLeft->SetModel(modelRunLeft);
	objectRunLeft->StopAnimation();

	//右に走るモデル
	modelRunRight = FbxLoader::GetInstance()->LoadModelFromFile("playerRunRight");
	//右に走るオブジェクト
	objectRunRight = new FbxObject3D;
	objectRunRight->Initialize();
	objectRunRight->SetModel(modelRunRight);
	objectRunRight->StopAnimation();

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
}

void Player::Update()
{
	//弾更新
	UpdateBullet();

	//挙動
	GameControl();

	//攻撃
	UpdateAttack();

	//ステータスマネージャー
	StatusManager();

	//オブジェクト更新
	UpdateObject();

	//1フレーム前の状態を代入
	preStatus = status;
}

void Player::UpdateTitle()
{
	//弾更新
	UpdateBullet();

	//挙動
	TitleControl();

	//攻撃
	UpdateAttack();

	//ステータスマネージャー
	StatusManager();

	//オブジェクト更新
	UpdateObject();

	//1フレーム前の状態を代入
	preStatus = status;
}

void Player::UpdateObject()
{
	UpdateObject(Wait, objectWait);
	UpdateObject(Run, objectRun);
	UpdateObject(BackRun, objectBackRun);
	UpdateObject(RunLeft, objectRunLeft);
	UpdateObject(RunRight, objectRunRight);
	UpdateObject(Attack1, objectAttack1);
	UpdateObject(Attack2, objectAttack2);
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

void Player::UpdateBullet()
{
	//if (input->TriggerKey(DIK_RETURN))
	//{
	//	//ショットフラグを立てる
	//	bullet->SetShotFlag(true);
	//	
	//	//弾ベクトル
	//	XMFLOAT3 bulletVelocity = rollRotation(XMFLOAT3(0.0f, 0.0f, 1.0f), rotation1);

	//	//弾生成場所とvelocityをセット
	//	bullet->SetBullet(position, bulletVelocity);
	//}
	//bullet->Update();
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
	if (status == BackRun)
	{
		objectBackRun->Draw(cmdList);
	}
	if (status == RunLeft)
	{
		objectRunLeft->Draw(cmdList);
	}
	if (status == RunRight)
	{
		objectRunRight->Draw(cmdList);
	}
	if (status == Attack1)
	{
		objectAttack1->Draw(cmdList);
	}
	if (status == Attack2)
	{
		objectAttack2->Draw(cmdList);
	}
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
	if (status == BackRun)
	{
		objectBackRun->DrawLightView(cmdList);
	}
	if (status == RunLeft)
	{
		objectRunLeft->DrawLightView(cmdList);
	}
	if (status == RunRight)
	{
		objectRunRight->DrawLightView(cmdList);
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

void Player::TitleControl()
{
	//ジャンプ更新
	UpdateJump();

	//重力更新
	UpdateGravity();

	//移動
	TitleMove();
}

void Player::GameControl()
{
	//ジャンプ更新
	UpdateJump();

	//重力更新
	UpdateGravity();

	//移動
	Move();
}

void Player::Move()
{
	//プレイヤーの元になる角度
	//AROWキーで角度変更
	rotVelocity.y = dxInput->GetStick(DXInput::RStickX) * rot0Speed;
	/*rotVelocity.y = (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * rotSpeed;*/
	//角度ベクトルを加算
	rotation0 = rotation0 + rotVelocity;

	//左スティック入力による角度変更
	rotation1.y = dxInput->GetStickRot(DXInput::LStick);

	//座標
	//ASDWで移動
	posVelocity.x = dxInput->GetStick(DXInput::LStickX) * posSpeed;
	posVelocity.z = dxInput->GetStick(DXInput::LStickY) * posSpeed;
	/*posVelocity.x = (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * posSpeed;
	posVelocity.z = (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * posSpeed;*/
	//進行ベクトルを回転
	posVelocity = rollRotation(posVelocity, rotation0);
	//進行ベクトルを加算
	position = position + posVelocity;
}

void Player::TitleMove()
{
	//AROWキーで角度変更
	rotVelocity.y = 0.01f;
	/*rotVelocity.y = (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * rotSpeed;*/
	//角度ベクトルを加算
	rotation1 = rotation1 + rotVelocity;

	//ASDWで移動
	posVelocity.x = 0.9;
	posVelocity.z = -0.1;
	/*posVelocity.x = (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * posSpeed;
	posVelocity.z = (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * posSpeed;*/
	//進行ベクトルを回転
	posVelocity = rollRotation(posVelocity, rotation1);
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
	if (dxInput->TriggerKey(DXInput::PAD_B) && Attack1Timer == 0)
	{
		status = Attack1;
	}
	else if (Attack1Timer >= 1)
	{
		status = Attack1;
	}

	else if (dxInput->TriggerKey(DXInput::PAD_B) && status == Attack1 &&
		0 <= Attack1Time - Attack1Timer <= Attack1IntervalTime)
	{
		status = Attack2;
	}
	else if (Attack2Timer >= 1)
	{
		status = Attack2;
	}

	//立ち止まっている場合
	else if (length(posVelocity) <= 0.01f)
	{
		status = Wait;
	}
	////後ろに入力しながら走っている場合
	//else if (dxInput->GetStick(DXInput::LStickY) <= -0.3)
	//{
	//	status = BackRun;
	//}
	////左に入力しながら走っている場合
	//else if (dxInput->GetStick(DXInput::LStickX) <= -0.3)
	//{
	//	status = RunLeft;
	//}
	////右に入力しながら走っている場合
	//else if (dxInput->GetStick(DXInput::LStickX) >= 0.3)
	//{
	//	status = RunRight;
	//}
	//普通に走っている場合
	else
	{
		status = Run;
	}
}

void Player::UpdateAttack()
{
	if (status == Attack1)
	{
		Attack1Timer++;
	}
	if (Attack1Timer >= Attack1Time)
	{
		Attack1Timer = 0.0f;
	}

	if (status == Attack2)
	{
		Attack2Timer++;
		Attack1Timer = 0;
	}
	if (Attack2Timer >= Attack2Time)
	{
		Attack2Timer = 0.0f;
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
	if (status == BackRun)
	{
		objectBackRun->SetSRV(SRV);
	}
	if (status == RunLeft)
	{
		objectRunLeft->SetSRV(SRV);
	}
	if (status == RunRight)
	{
		objectRunRight->SetSRV(SRV);
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