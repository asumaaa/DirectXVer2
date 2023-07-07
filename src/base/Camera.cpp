#include "Camera.h"
#include "Math.h"
#define PI 3.14159265359

Input* Camera::input = nullptr;
DXInput* Camera::dxInput = nullptr;

Camera* Camera::GetInstance()
{
	static Camera instance;
	return &instance;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	//�ˉe�ϊ�
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//�㉺��p45�x
		(float)window_width / window_height,//�A�X�y�N�g��(��ʉ���/��ʗ���)
		0.1f, 1000.0f						//�O�[�A���[
	);

	//�s��v�Z
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}


void Camera::Update()
{
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}

void Camera::DebugUpdate()
{
	//1�t���[��������̈ړ���
	float rot = (float)PI / 120.0f;

	//���_���W��ύX
	if (input->PushKey(DIK_LEFT))
	{
		DebugChangeRot -= (float)rot;
	}
	if (input->PushKey(DIK_RIGHT))
	{
		DebugChangeRot += (float)rot;
	}
	if (input->PushKey(DIK_UP))
	{
		DebugChangeRot2 -= (float)rot;
	}
	if (input->PushKey(DIK_DOWN))
	{
		DebugChangeRot2 += (float)rot;
	}

	//�^�[�Q�b�g�܂ł̋�����ύX
	if (input->PushKey(DIK_O))
	{
		DebugTargetDistance -= 0.2f;
	}
	if (input->PushKey(DIK_P))
	{
		DebugTargetDistance += 0.2f;
	}

	//���_���W�ɑ��
	eye_.x = sin(DebugChangeRot) * DebugTargetDistance + target_.x;
	eye_.y = sin(DebugChangeRot2) * DebugTargetDistance + target_.y;
	eye_.z = cos(DebugChangeRot) * DebugTargetDistance + target_.z;
}

void Camera::UpdatePlayer(XMFLOAT3 playerPos, XMFLOAT3 playerRot)
{
	target_ = { playerPos.x,0.0f,playerPos.z };

	/*eye_.x = sin(playerChangeRot) * playerTargetDistance + target_.x;
	eye_.y = sin(playerChangeRot2) * playerTargetDistance;
	eye_.z = cos(playerChangeRot) * playerTargetDistance + target_.z;*/

	eye_.x = playerPos.x + (cos(-playerRot.y - (PI / 2)) * playerTargetDistance);
	eye_.y = playerPos.y + (cos(-playerRot.x + (PI * 18 / 40)) * playerTargetDistance);
	eye_.z = playerPos.z + (sin(-playerRot.y - (PI / 2)) * playerTargetDistance);

}

void Camera::SetTarget(XMFLOAT3 pos)
{
	target_ = pos;
}

void Camera::SetEye(XMFLOAT3 pos)
{
	eye_ = pos;
}