/**
 * @file TextureManager.h
 * @brief ゲームで使うテクスチャをすべて管理
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once
#include "d3dx12.h"
#include "DirectXMath.h"
#include "array"
#include "DirectXTex.h"
#include "vector"

class TextureManager
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

public:	//メンバ関

	/// <summary>
	///初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///テクスチャを読み込む
	/// </summary>
	void LoadFile(int number, const wchar_t* fileName);

public:	//ゲッター

	/// <summary>
	///テクスチャのバッファ取得
	/// </summary>
	ID3D12Resource* GetTextureBuff(int number) { return textureBuff[number].Get(); }

	/// <summary>
	///シェーダリソースビュー取得
	/// </summary>
	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap.Get(); }

public:
	static void SetDevice(ID3D12Device* device) { TextureManager::device = device; }

private:	//静的メンバ変数

	/// <summary>
	///デバイスセット
	/// </summary>
	static ID3D12Device* device;

private:	//メンバ変数
	//テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSrvCount>textureBuff;
	//デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	std::vector<DirectX::TexMetadata> metadata;
	std::vector<DirectX::ScratchImage> scratchImg;
};
