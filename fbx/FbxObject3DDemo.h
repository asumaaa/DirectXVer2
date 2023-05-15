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

class FbxObject3DDemo
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
		XMFLOAT3 color;
	};
	//定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:	//静的メンバ関数
	//セッター
	static void SetDevice(ID3D12Device* device) { FbxObject3DDemo::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3DDemo::camera = camera; }
	static void SetLight(Light* light) { FbxObject3DDemo::light = light; }
	static void SetLightGroup(LightGroup* lightGroup) { FbxObject3DDemo::lightGroup = lightGroup; }

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
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//モデルのセット
	void SetModel(FbxModel* model) { this->model = model; }
	//グラフィックスパイプラインの生成
	static void CreateGraphicsPipeline();
	//アニメーション開始
	void PlayAnimation();

	//セッター
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 sca) { scale = sca; }
	void SetColor(XMFLOAT3 c) { color = c; }

private://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>constBuffTransform;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature0;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelinestate0;

private:
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z軸回りのローカル行列
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//モデル
	FbxModel* model = nullptr;

	//色
	XMFLOAT3 color = { 1.0f,1.0f,1.0f };

	//スペキュラー係数
	XMFLOAT3 specular = {0.1f,0.0f,0.0f};

	//定数バッファ
	ComPtr<ID3D12Resource>constBuffSkin;

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
};