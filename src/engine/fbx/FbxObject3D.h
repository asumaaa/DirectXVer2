#pragma once

#include "FbxModel.h"
#include "Camera.h"

#include "windows.h"
#include "wrl.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "DirectXMath.h"
#include "string.h"
#include "Light.h"
#include "LightGroup.h"
#include "JSONLoader.h"

class FbxObject3D
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//定数
	static const int MAX_BONES = 320;
	//サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		XMMATRIX lightviewproj;
	};
	//定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:	//静的メンバ関数
	//セッター
	static void SetDevice(ID3D12Device* device) { FbxObject3D::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3D::camera = camera; }
	static void SetLight(Light* light) { FbxObject3D::light = light; }
	static void SetLightGroup(LightGroup* lightGroup) { FbxObject3D::lightGroup = lightGroup; }

private://静的メンバ変数
	static ID3D12Device* device;
	static Camera* camera;
	static Light* light;
	static LightGroup* lightGroup;

public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
	void UpdateBillboard();
	void UpdateCollider();
	//描画
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//モデルのセット
	void SetModel(FbxModel* model) { this->model = model; }
	//ライト視点のグラフィックスパイプラインの生成
	static void CreateGraphicsPipelineLightView();
	//影付きカメラ視点のグラフィックスパイプラインの生成
	static void CreateGraphicsPipeline();
	//レベルエディタで設定したシェーダを使うグラフィックスパイプラインの生成
	void CreateGraphicsPipelineTexture1();	//テクスチャ1枚の場合
	void CreateGraphicsPipelineTexture2();	//テクスチャ2枚の場合
	void CreateGraphicsPipelineTexture3();	//テクスチャ3枚の場合
	void CreateGraphicsPipelineTexture4();	//テクスチャ4枚の場合

	//アニメーション開始
	void PlayAnimation();

	//セッター
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 sca) { scale = sca; }
	void SetSRV(ID3D12DescriptorHeap* SRV) { depthSRV = SRV; }
	void SetObjectData(JSONLoader::ObjectData objectData);
	void SetColliderData(JSONLoader::ColliderData colliderData);
	void SetTextureData(JSONLoader::TextureData textureData);
	void SetBillboardFlag() { billboardFlag = true; }
	void SetTextureNum(int textureNum) { this->textureNum1 = textureNum; }

	//ゲッター
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }
	std::string GetFileName() { return fileName; }
	std::string GetObjectName() { return objectName; }
	JSONLoader::ColliderData GetColliderData() { return colliderData; }

private://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>constBuffTransform;
	//ライト視点のルートシグネチャとパイプライン(shadowMap用)
	static ComPtr<ID3D12RootSignature>rootsignature0;
	static ComPtr<ID3D12PipelineState>pipelinestate0;
	//シェーダを指定する場合のルートシグネチャとパイプライン
	ComPtr<ID3D12RootSignature>rootsignature1;
	ComPtr<ID3D12PipelineState>pipelinestate1;
	//影付きカメラ視点のルートシグネチャとパイプライン
	static ComPtr<ID3D12RootSignature>rootsignature2;
	static ComPtr<ID3D12PipelineState>pipelinestate2;

private:
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z軸回りのローカル行列
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//ビルボード行列
	XMMATRIX matBillboard;
	//Y軸周りビルボード行列
	XMMATRIX matBillboiadY;
	//モデル
	FbxModel* model = nullptr;
	//コライダーの中心と座標の差分
	XMFLOAT3 colliderPos0 = { 0.0f,0.0f,0.0f };
	XMFLOAT3 colliderPos1 = {0.0f,0.0f,0.0f};
	XMFLOAT3 colliderCenter = { 0.0f,0.0f,0.0f };
	XMFLOAT3 colliderRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 colliderScale = { 0.0f,0.0f,0.0f };

	//ビルボードフラグ
	bool billboardFlag = false;

	//定数バッファ
	ComPtr<ID3D12Resource>constBuffSkin;

	//外部から受け取るSRV
	ID3D12DescriptorHeap* depthSRV;

	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;

	//ファイルの名前
	std::string fileName;
	//オブジェクトの名前
	std::string objectName;

	//コライダー
	JSONLoader::ColliderData colliderData;

	//テクスチャの番号
	int textureNum1 = 0;
	int textureNum2 = 0;
	int textureNum3 = 0;
	int textureNum4 = 0;
	//テクスチャの枚数
	int textureVol = 1;
	//使用するシェーダの名前
	std::string shaderName;
	bool shaderFlag = false;
};