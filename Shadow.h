#pragma once
#include "DirectXMath.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "vector"

class Shadow
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

public://サブクラス

//定数バッファ用データ構造体
	struct ConstBufferData
	{
		unsigned int active;
		XMMATRIX lightView;	//ライトのビュー座標
		XMMATRIX lightViewProj;	//ライトの射影変換行列
	};

public: //メンバ関数
	void SetLightPos(const XMFLOAT3& lightPos, const XMFLOAT3& target, const XMFLOAT3& up);	//ライトの座標を取得してビュー行列計算
	inline const XMFLOAT3& GetLightPos() { return lightPos; }
	inline void SetLightView(const XMMATRIX& lightView) { this->lightView = lightView; }
	inline const XMMATRIX& GetLightView() { return lightView; }
	inline void SetLightViewProj(const XMMATRIX& lightViewProj) { this->lightViewProj = lightViewProj; }
	inline const XMMATRIX& GetLightViewProj() { return lightViewProj; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }


private:	//メンバ変数

	//// 方向 (単位ベクトル)
	//XMVECTOR dir = { 1,0,0,0 };
	////キャスターとライトの距離
	//float distanceCasterLight = 100.0f;
	//// キャスター座標(ワールド座標系)
	//XMFLOAT3 casterPos = { 0,0,0 };
	//// 距離減衰係数
	//XMFLOAT3 atten = { 0.5f, 0.6f, 0.0f };
	//// 減衰角度
	//XMFLOAT2 factorAngleCos = { 0.2f, 0.5f };
	
	XMFLOAT3 lightPos;	//ライトの座標
	XMMATRIX lightView;	//ライトのビュー座標
	XMMATRIX lightViewProj;	//ライトの射影変換行列
	// 有効フラグ
	bool active = false;

};



