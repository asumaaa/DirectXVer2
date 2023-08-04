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
	//�e�X�V
	UpdateBullet();

	//����
	Move();

	//�I�u�W�F�N�g�X�V
	UpdateObject();
}

void Player::UpdateObject()
{
	object->SetPosition(position);
	object->SetRotation(rotation0 + rotation1);
	object->SetScale(scale);

	object->Update();
}

void Player::UpdateBullet()
{
	if (input->TriggerKey(DIK_RETURN))
	{
		//�V���b�g�t���O�𗧂Ă�
		bullet->SetShotFlag(true);
		
		//�e�x�N�g��
		XMFLOAT3 bulletVelocity = rollRotation(XMFLOAT3(0.0f, 0.0f, 1.0f), rotation1);

		//�e�����ꏊ��velocity���Z�b�g
		bullet->SetBullet(position, bulletVelocity);
	}
	bullet->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	bullet->Draw(cmdList);
}

void Player::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	bullet->DrawLightView(cmdList);
}

void Player::Move()
{
	//�W�����v�X�V
	UpdateJump();

	//�d�͍X�V
	UpdateGravity();
	
	//�L�[����
	KeyControl();
}

void Player::KeyControl()
{
	//AROW�L�[�Ŋp�x�ύX
	rotVelocity.y = (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * rotSpeed;
	//�p�x�x�N�g�������Z
	rotation1 = rotation1 + rotVelocity;

	//ASDW�ňړ�
	posVelocity.x = (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * posSpeed;
	posVelocity.z = (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * posSpeed;
	//�i�s�x�N�g������]
	posVelocity = rollRotation(posVelocity, rotation1);
	//�i�s�x�N�g�������Z
	position = position + posVelocity;
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
	position = position + fallVelocity;
}

void Player::UpdateJump()
{
	//�ڒn���Ă�����
	if (groundFlag == true)
	{
		//�X�y�[�X�L�[�ŃW�����v
		if (input->TriggerKey(DIK_SPACE))
		{
			groundFlag = false;
			fallTimer = -jumpHeight;
			fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}
}

void Player::UpdateAttack()
{
}

void Player::SetObject(FbxObject3D* object)
{
	//�����̃I�u�W�F�N�g���Z�b�g
	Player::object.reset(object);

	position = object->GetPosition();
	rotation0 = object->GetRotation();
	scale = object->GetScale();
}

void Player::SetSRV(ID3D12DescriptorHeap* SRV)
{
	bullet->SetSRV(SRV);
}

void Player::HitPlane()
{
	//�ڒn�t���O�𗧂Ă�
	groundFlag = true;

	//�߂荞�܂Ȃ��Ȃ�܂ŉ��Z
	position.y += 0.1f;

	//�I�u�W�F�N�g�X�V
	UpdateObject();
}