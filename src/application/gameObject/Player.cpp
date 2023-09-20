#include "Player.h"
#include "mathOriginal.h"
#include "FbxLoader.h"
#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

Camera* Player::camera = nullptr;
Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

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
}

void Player::Update()
{
	//弾更新
	UpdateBullet();

	//動く
	Move();

	//オブジェクト更新
	UpdateObject();

	//1フレーム前の状態を代入
	preStatus = status;
}

void Player::UpdateObject()
{
	//待ってるオブジェクト
	if (status == Wait)
	{
		objectWait->SetPosition(position);
		objectWait->SetRotation(rotation0 + rotation1);
		objectWait->SetScale(scale);
		if (status != preStatus)
		{
			objectWait->PlayAnimation();
		}

		objectWait->Update();
	}
	else
	{
		objectWait->StopAnimation();
	}

	//走ってるオブジェクト
	if (status == Run)
	{
		objectRun->SetPosition(position);
		objectRun->SetRotation(rotation0 + rotation1);
		objectRun->SetScale(scale);
		if (status != preStatus)
		{
			objectRun->PlayAnimation();
		}

		objectRun->Update();
	}
	else
	{
		objectRun->StopAnimation();
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
}

void Player::Move()
{
	//ジャンプ更新
	UpdateJump();

	//重力更新
	UpdateGravity();
	
	//キー操作
	KeyControl();
}

void Player::KeyControl()
{
	//AROWキーで角度変更
	rotVelocity.y = (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * rotSpeed;
	//角度ベクトルを加算
	rotation1 = rotation1 + rotVelocity;

	//ASDWで移動
	posVelocity.x = (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * posSpeed;
	posVelocity.z = (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * posSpeed;
	//進行ベクトルを回転
	posVelocity = rollRotation(posVelocity, rotation1);
	//進行ベクトルを加算
	position = position + posVelocity;

	//進行速度によってステータスを変更
	if (length(posVelocity) >= 0.01f)
	{
		status = Run;
	}
	else
	{
		status = Wait;
	}
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

void Player::UpdateAttack()
{
}

void Player::SetSRV(ID3D12DescriptorHeap* SRV)
{
	objectWait->SetSRV(SRV);
	if (status == Wait)
	{
		objectWait->SetSRV(SRV);
	}
	if (status == Run)
	{
		objectRun->SetSRV(SRV);
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
