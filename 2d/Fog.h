#pragma once
#include "d3dx12.h"
#include "array"
#include "DirectXMath.h"
#include "DirectXTex.h"
#include "vector"

class Fog
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	 //定数
	//SRVの最大個数
	static const size_t kMaxSrvCount = 2056;

public:	//サブクラス
	//定数バッファ
	struct ConstBuffMaterial
	{
		//色
		XMFLOAT4 color;
		//解像度
		float resolution;
		//ウィンドウ
		XMFLOAT2 window;
		//フォグをかけ始める位置
		float startDepth;
		//フォグの強さ
		float strength;
	};
	//定数バッファ2
	struct ConstBuffTransform
	{
		XMMATRIX mat;	//3D変換行列
	};
	//頂点データ用構造体
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

public:	//メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//パイプライン設定、作成
	void CreateGraphicsPipeLine0();

	//描画前処理
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	//描画後処理
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

public:	//静的メンバ関数
	static void SetDevice(ID3D12Device* device) { Fog::device = device; }

public:	//セッター
	//アルファ値
	void SetAlpha(float alpha) { color.w = alpha; }
	//色
	void SetColor(XMFLOAT3 c) { color.x = c.x; color.y = c.y; color.z = c.z; }
	//座標
	void SetPosition(XMFLOAT2 pos) { position = pos; }
	//角度
	void SetRotation(float rot) { rotation = rot; }
	//スケール
	void SetScale(XMFLOAT2 sca) { scale = sca; }
	//解像度
	void SetResolution(float re) { resolution = re; }
	//フォグをかけ始める位置
	void SetStartDepth(float s) { startDepth = s; }
	//フォグの強さ
	void SetStrength(float f) { strength = f; }

private:	//静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelinestate;
	//画面クリアカラー
	static const float clearColor[4];

private:	//メンバ変数
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//頂点データ
	Vertex vertices[6];
	Vertex* vertMap = nullptr;
	//定数バッファ マテリアル
	ComPtr<ID3D12Resource>constBuffMaterial;
	ConstBuffMaterial* constMapMaterial = nullptr;
	//定数バッファ 変形行列
	ComPtr<ID3D12Resource>constBuffTransform;
	ConstBuffTransform* constMapTransform = nullptr;
	//テクスチャの色
	XMFLOAT4 color = { 1,1,1,1 };
	//テクスチャバッファ
	ComPtr<ID3D12Resource>textureBuff;
	//デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//深度バッファ
	ComPtr<ID3D12Resource>depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descHeapDSV;
	//深度値テクスチャ用
	ComPtr<ID3D12DescriptorHeap>depthSRVHeap;

private:
	//解像度
	float resolution = 10;
	float rotation = 0;
	XMFLOAT2 position = { 0,0 };
	XMFLOAT2 scale = { 100.0f,100.0f };
	//フォグをかけ始める深度地
	float startDepth = 0.2f;
	//フォグの強さ
	float strength = 1.0f;
};
