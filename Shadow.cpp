#include "Shadow.h"
#include "WinApp.h"

void Shadow::SetLightPos(const XMFLOAT3& lightPos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	//射影変換
	lightViewProj = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(45.0f),			//上下画角45度
		(float)window_width / window_height,//アスペクト比(画面横幅/画面立幅)
		0.1f, 1000.0f						//前端、奥端
	);

	//行列計算
	lightView = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&lightPos), 
		DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
}
