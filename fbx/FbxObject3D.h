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
		float alpha;
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
	//描画
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawStandard(ID3D12GraphicsCommandList* cmdList);	//影なし描画をする場合
	//モデルのセット
	void SetModel(FbxModel* model) { this->model = model; }
	//グラフィックスパイプラインの生成
	static void CreateGraphicsPipelineLightView();
	static void CreateGraphicsPipeline0();
	static void CreateGraphicsPipeline1();
	//アニメーション開始
	void PlayAnimation();
	//アニメーション停止
	void StopAnimation();
	//アニメーション再開
	void RestartAnimation();

	//セッター
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 sca) { scale = sca; }
	void SetSRV(ID3D12DescriptorHeap* SRV) { depthSRV = SRV; }
	void SetAlpha(float f) { alpha = f; }

private://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>constBuffTransform;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature0;
	static ComPtr<ID3D12RootSignature>rootsignature1;
	static ComPtr<ID3D12RootSignature>rootsignature2;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelinestate0;
	static ComPtr<ID3D12PipelineState>pipelinestate1;
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
	//モデル
	FbxModel* model = nullptr;

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

	float alpha = 1.0f;
};