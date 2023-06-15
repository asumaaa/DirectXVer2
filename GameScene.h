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
#include "CSVLoader.h"
#include "Player.h"
#include "ColliderCubeObject.h"
#include "ColliderSphereObject.h"
#include "ColliderPlaneObject.h"
#include "ColliderManager.h"

class GameScene
{
	//メンバ関数
public:
	GameScene();
	~GameScene();
	//初期化
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//終了時
	void Finalize();
	//更新
	void Update();
	void UpdateCollider();
	//描画
	void Draw();
	void DrawFBXLightView();
	void DrawFBX();
	void DrawCollider();
	
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
	std::list<std::unique_ptr<FbxModel>> models;

	//レベルエディタ
	std::unique_ptr<JSONLoader> jsonLoader;

	//オブジェクト
	std::list<std::unique_ptr<FbxObject3D>> object;

	//ライト 影用
	std::unique_ptr<Light> light;
	float lightDir[3] = {0.0f,-1.0f , -1.0f};
	float lightPos[3] = {0.0f,25.0f,25.0f};
	float lightTarget[3] = { 0.0f,0.0f,0.0f };
	float lightFactorAngle[2] = { 20.0f,30.0f, };
	float lightAtten[3] = {0.0f,0.0f,0.0f};

	//ライト
	std::unique_ptr<LightGroup> lightGroup;
	float lightManagerDir[3] = { 0.0f,-1.0f , 1.0f };

	//スプライトマネージャー
	std::unique_ptr <SpriteManager> spriteManager;

	//変形行列
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };

	//プレイヤー
	std::unique_ptr<Player> player;
	
	//平面
	/*std::unique_ptr<Plane> plane;*/

	//コライダーのモデル
	std::unique_ptr<ColliderCubeModel>colliderCubeModel;
	std::unique_ptr<ColliderSphereModel>colliderSphereModel;
	std::unique_ptr<ColliderPlaneModel>colliderPlaneModel;

	//コライダー
	std::unique_ptr<ColliderManager> colliderManager;
};
