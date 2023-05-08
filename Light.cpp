#include "Light.h"

void Light::Initialize()
{
	//射影変換
	//matProjection_ = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),			//上下画角45度
	//	(float)window_width / window_height,//アスペクト比(画面横幅/画面立幅)
	//	0.1f, 1000.0f						//前端、奥端
	//);

	matProjection_ = XMMatrixOrthographicLH(
		80,80,1.0f,200.0f);

	//行列計算
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}

void Light::Update()
{
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}
