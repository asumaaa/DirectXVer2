#pragma once
#include "d3dx12.h"
#include "DirectXMath.h"

class Sprite
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//サブクラス
	//定数バッファ
	struct ConstBuff
	{
		XMFLOAT4 color;
	};
	//頂点データ用構造体
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

public:	//メンバ関数
	void Initialize();
	void LoadFile(const wchar_t* fileName);
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:	//静的メンバ変数セッター
	static void SetDevice(ID3D12Device* device) { Sprite::device = device; }
	static void CreateGraphicsPipeLine();

public:
	void SetAlpha(float alpha) { color.w = alpha; }

private:
	//デバイス
	static ID3D12Device* device;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelinestate;

private:	//メンバ変数
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//頂点データ
	Vertex vertices[6];
	//定数バッファ
	ComPtr<ID3D12Resource>constBuffMaterial;
	//テクスチャバッファ
	ComPtr<ID3D12Resource>textureBuff;
	//デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	//テクスチャの色
	XMFLOAT4 color = {1,1,1,1};
};

