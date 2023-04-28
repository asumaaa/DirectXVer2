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
	//木
	FbxModel* modelTree = nullptr;
	std::list<std::unique_ptr<FbxObject3D>> objectTree;
	//木の数
	size_t verticalTreeNum = 3;
	size_t horizonTreeNum = 6;
	//木の幅
	float verticalTreeWidth = 10.0f;
	float horizonTreeWidth = 9.0f;
	size_t treeNum = verticalTreeNum * horizonTreeNum;
	//木の変形行列
	XMFLOAT3 treeScale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 treeRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 treePosition = { 0.0f,0.0f,0.0f };

	//床
	FbxModel* model1 = nullptr;
	FbxObject3D* object1 = nullptr;
	FbxModel* model2 = nullptr;

	//ライト
	Light* light = nullptr;

	//スプライトマネージャー
	SpriteManager* spriteManager = nullptr;

	//変形行列
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };
};
