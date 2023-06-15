#include "Player.h"
#include "mathOriginal.h"
#define G 6.674	//���L���͒萔
#define GAcceleration 9.80665 * 1/10	//�d�͉����x

Camera* Player::camera = nullptr;
Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

void Player::Initialize()
{
}

void Player::Update()
{
	//����
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
	////�W�����v�X�V
	//UpdateJump();

	////�d�͍X�V
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
	//�ڒn���Ă�����^�C�}�[�ƃx�N�g�����Z�b�g
	if (groundFlag == true)
	{
		fallTimer = 0.0f;
		fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	//�ڒn���Ă��Ȃ����
	if (groundFlag == false)
	{
		//�����^�C�}�[���ő�l��菬�������
		if (fallTimer < fallTime)
		{
			fallTimer += fallFrame;
		}
	}

	//�����x�N�g���v�Z
	fallVelocity.y = -(GAcceleration * fallTimer);

	//���W�ɗ����x�N�g�������Z
	position.x += fallVelocity.x;
	position.y += fallVelocity.y;
	position.z += fallVelocity.z;
}

void Player::UpdateJump()
{
	//�X�y�[�X�L�[�ŃW�����v
	if (input->TriggerKey(DIK_SPACE))
	{
		groundFlag = false;
		fallTimer = -jumpHeight;
		fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
}

void Player::SetObject(FbxObject3D* object)
{
	//�����̃I�u�W�F�N�g���Z�b�g
	Player::object.reset(object);

	position = object->GetPosition();
	rotation = object->GetRotation();
	scale = object->GetScale();
}

void Player::SetSRV(ID3D12DescriptorHeap* SRV)
{
	object->SetSRV(SRV);
}
