#pragma once
#include "d3dx12.h"
#include "DirectXMath.h"

class SpriteManager
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//メンバ関数
	void LoadFile(int number,const wchar_t* fileName);

public:	//ゲッター
	ID3D12Resource* GetTextureBuff() { return textureBuff.Get(); }
	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap.Get(); }

public:
	static void SetDevice(ID3D12Device* device) { SpriteManager::device = device; }

private:	//静的メンバ変数
	//デバイス
	static ID3D12Device* device;

private:	//メンバ変数
	//テクスチャバッファ
	ComPtr<ID3D12Resource>textureBuff;
	//デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap;
};

