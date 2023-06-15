#include "Player.h"
#include "mathOriginal.h"
#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

Camera* Player::camera = nullptr;
Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

void Player::Initialize()
{
}

void Player::Update()
{
	//動く
	Move();

	object->SetPosition(position);
	object->SetRotation(rotation);
	object->SetScale(scale);

	object->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object->Draw(cmdList);
}

void Player::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	object->DrawLightView(cmdList);
}

void Player::Move()
{
	////ジャンプ更新
	//UpdateJump();

	////重力更新
	//UpdateGravity();

	position.x -= input->PushKey(DIK_A) * 0.1f;
	position.x += input->PushKey(DIK_D) * 0.1f;
	position.y -= input->PushKey(DIK_Z) * 0.1f;
	position.y += input->PushKey(DIK_X) * 0.1f;
	position.z -= input->PushKey(DIK_S) * 0.1f;
	position.z += input->PushKey(DIK_W) * 0.1f;
}

void Player::UpdateGravity()
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

void Player::UpdateJump()
{
	//スペースキーでジャンプ
	if (input->TriggerKey(DIK_SPACE))
	{
		groundFlag = false;
		fallTimer = -jumpHeight;
		fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
}

void Player::SetObject(FbxObject3D* object)
{
	//引数のオブジェクトをセット
	Player::object.reset(object);

	position = object->GetPosition();
	rotation = object->GetRotation();
	scale = object->GetScale();
}

void Player::SetSRV(ID3D12DescriptorHeap* SRV)
{
	object->SetSRV(SRV);
}
