#pragma once

#include "DirectXMath.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "vector"
#include "WinApp.h"

class ShadowMap
{
private: // エイリアス 
	// Microsoft::WRLを省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//サブクラス
	struct ConstBufferDate
	{
		XMMATRIX lightViewProjection;
		XMFLOAT4 lightColor;
		XMFLOAT3 lightDir;
	};

public:	//メンバ関数
	//デバイスをセット
	static void SetDevice(ID3D12Device* device) { ShadowMap::device = device; }
	//ライトの座標をセット
	inline void SetLightPos(const XMFLOAT3& lightPos, const XMFLOAT3& target, const XMFLOAT3& up);
	//ライトの方向をセット
	inline void SetLightDir(XMFLOAT3 lightDir) { this->lightDir = lightDir; }

public:
	void CreateBuffer();
	void Update();
	void Draw();

private://静的メンバ変数
	static ID3D12Device* device;

private:	//メンバ変数
	//ライトの座標
	XMFLOAT3 lightPos;
	//ライト
	XMMATRIX lightViewProjection;
	XMMATRIX lightView;
	XMFLOAT4 lightColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT3 lightDir;
};

