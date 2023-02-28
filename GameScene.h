#pragma once
#include "memory"
#include "list"

#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "LightGroup.h"

class GameScene
{
	//メンバ関数
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//更新
	void Update();
	//描画
	void Draw();

	//メンバ変数
private:
	//デバイスとinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	//カメラ
	std::unique_ptr<Camera> camera_;

	//fbx
	FbxModel* model0 = nullptr;
	FbxObject3D* object0 = nullptr;
	FbxObject3D* object2 = nullptr;

	FbxModel* model1 = nullptr;
	FbxObject3D* object1 = nullptr;
	FbxModel* model2 = nullptr;

	//ライト
	LightGroup* lightGroup = nullptr;

	float ambientColor0[3] = { 1,1,1 };
	//光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 0,0,0 };

	float pointLightPos0[3] = { 0,0,0 };
	float pointLightColor0[3] = { 1,1,1 };
	float pointLightAtten0[3] = {0.3f,0.1f,0.1f};

	float circleShadowDir[3] = { 1,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f,0.5f };

	//変形行列
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };
};
