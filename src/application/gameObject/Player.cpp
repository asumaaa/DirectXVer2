#include "Player.h"
#include "mathOriginal.h"
#include "FbxLoader.h"
#define G 6.674	//���L���͒萔
#define GAcceleration 9.80665 * 1/10	//�d�͉����x

Camera* Player::camera = nullptr;
Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

Player::~Player()
{
	//�I�u�W�F�N�g�̍폜
	delete objectWait;
	delete objectRun;
	delete objectBackRun;
	delete objectRunLeft;
	delete objectRunRight;
	delete objectAttack1;
	delete objectAttack2;

	//���f���̍폜
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
	//�҂��Ă郂�f��
	modelWait = FbxLoader::GetInstance()->LoadModelFromFile("playerWait");
	//�҂��Ă���I�u�W�F�N�g
	objectWait = new FbxObject3D;
	objectWait->Initialize();
	objectWait->SetModel(modelWait);
	objectWait->PlayAnimation();

	//�����Ă郂�f��
	modelRun = FbxLoader::GetInstance()->LoadModelFromFile("playerRun");
	//�����Ă�I�u�W�F�N�g
	objectRun = new FbxObject3D;
	objectRun->Initialize();
	objectRun->SetModel(modelRun);
	objectRun->StopAnimation();

	//����ɑ��郂�f��
	modelBackRun = FbxLoader::GetInstance()->LoadModelFromFile("playerBackRun");
	//����ɑ���I�u�W�F�N�g
	objectBackRun = new FbxObject3D;
	objectBackRun->Initialize();
	objectBackRun->SetModel(modelBackRun);
	objectBackRun->StopAnimation();

	//���ɑ��郂�f��
	modelRunLeft = FbxLoader::GetInstance()->LoadModelFromFile("playerRunLeft");
	//���ɑ���I�u�W�F�N�g
	objectRunLeft = new FbxObject3D;
	objectRunLeft->Initialize();
	objectRunLeft->SetModel(modelRunLeft);
	objectRunLeft->StopAnimation();

	//�E�ɑ��郂�f��
	modelRunRight = FbxLoader::GetInstance()->LoadModelFromFile("playerRunRight");
	//�E�ɑ���I�u�W�F�N�g
	objectRunRight = new FbxObject3D;
	objectRunRight->Initialize();
	objectRunRight->SetModel(modelRunRight);
	objectRunRight->StopAnimation();

	//�U��1�̃��f��
	modelAttack1 = FbxLoader::GetInstance()->LoadModelFromFile("playerAttack1");
	//�U��1�̃I�u�W�F�N�g
	objectAttack1 = new FbxObject3D;
	objectAttack1->Initialize();
	objectAttack1->SetModel(modelAttack1);
	objectAttack1->StopAnimation();

	//�U��2�̃��f��
	modelAttack2 = FbxLoader::GetInstance()->LoadModelFromFile("playerAttack2");
	//�U��2�̃I�u�W�F�N�g
	objectAttack2 = new FbxObject3D;
	objectAttack2->Initialize();
	objectAttack2->SetModel(modelAttack2);
	objectAttack2->StopAnimation();
}

void Player::Update()
{
	//�e�X�V
	UpdateBullet();

	//����
	Control();

	//�U��
	UpdateAttack();

	//�X�e�[�^�X�}�l�[�W���[
	StatusManager();

	//�I�u�W�F�N�g�X�V
	UpdateObject();

	//1�t���[���O�̏�Ԃ���
	preStatus = status;
}

void Player::UpdateObject()
{
	UpdateObject(Wait, objectWait);
	UpdateObject(Run, objectRun);
	UpdateObject(BackRun, objectBackRun);
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
	//	//�V���b�g�t���O�𗧂Ă�
	//	bullet->SetShotFlag(true);
	//	
	//	//�e�x�N�g��
	//	XMFLOAT3 bulletVelocity = rollRotation(XMFLOAT3(0.0f, 0.0f, 1.0f), rotation1);

	//	//�e�����ꏊ��velocity���Z�b�g
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
	if (status == Attack1)
	{
		objectAttack1->DrawLightView(cmdList);
	}
	if (status == Attack2)
	{
		objectAttack2->DrawLightView(cmdList);
	}
}

void Player::Control()
{
	//�W�����v�X�V
	UpdateJump();

	//�d�͍X�V
	UpdateGravity();

	//�ړ�
	Move();
}

void Player::Move()
{
	//AROW�L�[�Ŋp�x�ύX
	rotVelocity.y = dxInput->GetStick(DXInput::RStickX) * rotSpeed;
	/*rotVelocity.y = (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * rotSpeed;*/
	//�p�x�x�N�g�������Z
	rotation1 = rotation1 + rotVelocity;

	//ASDW�ňړ�
	posVelocity.x = dxInput->GetStick(DXInput::LStickX) * posSpeed;
	posVelocity.z = dxInput->GetStick(DXInput::LStickY) * posSpeed;
	/*posVelocity.x = (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * posSpeed;
	posVelocity.z = (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * posSpeed;*/
	//�i�s�x�N�g������]
	posVelocity = rollRotation(posVelocity, rotation1);
	//�i�s�x�N�g�������Z
	position = position + posVelocity;
}

void Player::UpdateGravity()
{
	////�ڒn���Ă�����^�C�}�[�ƃx�N�g�����Z�b�g
	//if (groundFlag == true)
	//{
	//	fallTimer = 0.0f;
	//	fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//}

	////�ڒn���Ă��Ȃ����
	//if (groundFlag == false)
	//{
	//	//�����^�C�}�[���ő�l��菬�������
	//	if (fallTimer < fallTime)
	//	{
	//		fallTimer += fallFrame;
	//	}
	//}

	////�����x�N�g���v�Z
	//fallVelocity.y = -(GAcceleration * fallTimer);

	////���W�ɗ����x�N�g�������Z
	//position = position + fallVelocity;
}

void Player::UpdateJump()
{
	////�ڒn���Ă�����
	//if (groundFlag == true)
	//{
	//	//�X�y�[�X�L�[�ŃW�����v
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
	else if(Attack1Timer >= 1)
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

	//�����~�܂��Ă���ꍇ
	else if (length(posVelocity) <= 0.01f)
	{
		status = Wait;
	}
	//���ɓ��͂��Ȃ��瑖���Ă���ꍇ
	else if (dxInput->GetStick(DXInput::LStickY) <= -0.3)
	{
		status = BackRun;
	}
	//���ʂɑ����Ă���ꍇ
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
	//�ڒn�t���O�𗧂Ă�
	groundFlag = true;

	//�߂荞�܂Ȃ��Ȃ�܂ŉ��Z
	position.y += 0.1f;

	//�I�u�W�F�N�g�X�V
	UpdateObject();
}
