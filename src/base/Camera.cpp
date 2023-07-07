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
	//射影変換
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//上下画角45度
		(float)window_width / window_height,//アスペクト比(画面横幅/画面立幅)
		0.1f, 1000.0f						//前端、奥端
	);

	//行列計算
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}


void Camera::Update()
{
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}

void Camera::DebugUpdate()
{
	//1フレームあたりの移動量
	float rot = (float)PI / 120.0f;

	//視点座標を変更
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

	//ターゲットまでの距離を変更
	if (input->PushKey(DIK_O))
	{
		DebugTargetDistance -= 0.2f;
	}
	if (input->PushKey(DIK_P))
	{
		DebugTargetDistance += 0.2f;
	}

	//視点座標に代入
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