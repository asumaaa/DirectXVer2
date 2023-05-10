#pragma once
#include "memory"
#include "list"

#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "FireParticle.h"
#include "Light.h"
#include "LightGroup.h"
#include "VolumeLightModel.h"
#include "VolumeLightObject.h"

#define PI 3.1415

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
	void DrawFBXLightView();
	void DrawFBX();
	
	//セッター
	void SetSRV(ID3D12DescriptorHeap* SRV);
	//ゲッター
	DirectX::XMMATRIX GetLightViewProjection();

	//メンバ変数
private:
	//デバイスとinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	//カメラ
	std::unique_ptr<Camera> camera_;

	//fbx
	//石
	FbxModel* modelStone = nullptr;
	std::list<std::unique_ptr<FbxObject3D>> objectStone;
	//石の数
	size_t verticalStoneNum = 10;
	size_t horizonStoneNum = 10;
	//石の幅
	float verticalStoneWidth = 10.0f;
	float horizonStoneWidth = 9.0f;
	size_t treeNum = verticalStoneNum * horizonStoneNum;
	//石の変形行列
	XMFLOAT3 stoneScale = { 2.0f,2.0f,2.0f };
	XMFLOAT3 stoneRotation = { 1.5f * PI,0.0f,0.0f };
	XMFLOAT3 stonePosition = { 0.0f,0.0f,0.0f };

	//木
	FbxModel* modelTree = nullptr;
	FbxObject3D* objectTree = nullptr;
	//木の変形行列
	XMFLOAT3 treePosition = { 0.0f,2.0f,5.0f };
	XMFLOAT3 treeRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 treeScale = {5.0f,5.0f,5.0f };

	//床
	FbxModel* model1 = nullptr;
	FbxObject3D* object1 = nullptr;
	FbxModel* model2 = nullptr;

	//ライト 影用
	Light* light = nullptr;
	float lightDir[3] = {0.0f,-1.0f , -1.0f};
	float lightPos[3] = {0.0f,25.0f,25.0f};
	float lightTarget[3] = { 0.0f,0.0f,0.0f };
	float lightFactorAngle[2] = { 20.0f,30.0f, };
	float lightAtten[3] = {0.0f,0.0f,0.0f};

	//ライト
	LightGroup* lightGroup = nullptr;
	float lightManagerDir[3] = { 0.0f,-1.0f , 1.0f };

	//ボリュームライト
	std::unique_ptr <VolumeLightModel> volumeLightModel;
	std::unique_ptr <VolumeLightObject> volumeLightObject;
	float volumeLightPos[3] = { 0.0f,5.0f,-10.0f };
	float volumeLightScale[3] = { 4.0f,4.0f,4.0f };
	float volumeLightRotation[3] = { 0.0f,0.0f,0.0f };

	//スプライトマネージャー
	SpriteManager* spriteManager = nullptr;

	//変形行列
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };
};
