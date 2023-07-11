#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"

class Plane
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	//メンバ関数
public:
	//静的メンバ関数
	static void SetCamera(Camera* camera) { Plane::camera = camera; }
	static void SetInput(Input* input) { Plane::input = input; }
	static void SetDXInput(DXInput* dxInput) { Plane::dxInput = dxInput; }

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//移動
	void Move();

	//セッター
	void SetObject(FbxObject3D* object);
	void SetSRV(ID3D12DescriptorHeap* SRV);

	//ゲッター
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }

	//静的メンバ変数
private:
	//カメラ
	static Camera* camera;
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;

	//メンバ変数
public:

	//オブジェクト
	std::list<std::unique_ptr<FbxObject3D>>object;

	//座標
	XMFLOAT3 position = {0.0f,0.0f,0.0f};
	//回転
	XMFLOAT3 rotation = {0.0f,0.0f,0.0f};
	//サイズ
	XMFLOAT3 scale = {1.0f,1.0f,1.0f};
};

