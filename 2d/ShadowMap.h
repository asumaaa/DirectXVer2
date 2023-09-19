#pragma once
#include "d3dx12.h"
#include "array"
#include "DirectXMath.h"
#include "DirectXTex.h"
#include "vector"

class ShadowMap
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
		//ライトのビュー
		XMMATRIX lightvp;
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
	//終了時
	void Finalize();
	//更新
	void Update();
	//描画
	void Draw0(ID3D12GraphicsCommandList* cmdList);
	void Draw1(ID3D12GraphicsCommandList* cmdList);
	//パイプライン設定、作成
	void CreateGraphicsPipeLine0();
	void CreateGraphicsPipeLine1();

	//描画前処理
	void PreDrawScene0(ID3D12GraphicsCommandList* cmdList);
	//描画後処理
	void PostDrawScene0(ID3D12GraphicsCommandList* cmdList);

public:	//静的メンバ関数
	static void SetDevice(ID3D12Device* device) { ShadowMap::device = device; }

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
	//ビュー
	void SetLightVP(XMMATRIX l) { lightVP = l; }

	//ゲッター
	ID3D12DescriptorHeap* GetSRV() { return depthSRVHeap.Get(); }

private:	//静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature0;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelinestate0;
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
	//shadowmap用深度バッファ ライト目線
	ComPtr<ID3D12Resource>lightDepthBuff;

private:
	//解像度
	float resolution = 10;
	float rotation = 0;
	XMFLOAT2 position = { 0,0 };
	XMFLOAT2 scale = { 100.0f,100.0f };
	//射影変換
	XMMATRIX lightVP;
};

