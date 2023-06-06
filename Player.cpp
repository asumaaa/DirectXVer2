#include "Player.h"

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
	position.x -= input->PushKey(DIK_A) * 0.2f;
	position.x += input->PushKey(DIK_D) * 0.2f;
	position.y -= input->PushKey(DIK_Z) * 0.2f;
	position.y += input->PushKey(DIK_X) * 0.2f;
	position.z -= input->PushKey(DIK_S) * 0.2f;
	position.z += input->PushKey(DIK_W) * 0.2f;
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
