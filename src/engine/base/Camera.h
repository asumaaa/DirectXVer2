#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "DXInput.h"
#include "WinApp.h"
//#include "Player.h"
#define PI 3.14159265359

using namespace DirectX;

class Camera
{
public:

public:
	//シングルトンインスタンス
	static Camera* GetInstance();
	static void SetInput(Input* input) { Camera::input = input; }
	static void SetDXInput(DXInput* dxInput) { Camera::dxInput = dxInput; }
	//インストラクタ デストラクタ
	Camera();
	~Camera();
	//初期化
	void Initialize();
	//更新
	void Update();
	//ビルボード行列の更新
	void BillboardUpdate();
	//デバッグ Arrowキーで視点座標変更
	void DebugUpdate();
	//プレイヤー追尾
	void UpdatePlayer(XMFLOAT3 playerPos,XMFLOAT3 playerRot);

	//ゲッターセッター
	void SetTarget(XMFLOAT3 pos);
	void SetEye(XMFLOAT3 pos);
	XMFLOAT3 GetEye() { return eye_; };
	XMFLOAT3 GetTarget() { return target_; };
	XMFLOAT3 GetUp() { return up_; };
	XMMATRIX GetMatProjection() { return matProjection_; };
	XMMATRIX GetMatView() { return matView_; };
	XMMATRIX GetMatViewProjection() { return matView_ * matProjection_; };
	XMMATRIX GetMatBillboard() { return matBillboard_; }

private:
	//入力
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	//射影変換
	XMMATRIX matProjection_ = XMMatrixIdentity();
	//ビュー変換行列
	XMMATRIX matView_ = XMMatrixIdentity();
	XMFLOAT3 eye_ = { 0, 20, -100 };
	XMFLOAT3 target_ = { 0, 0, 0 };
	XMFLOAT3 up_ = { 0, 1, 0 };
	//ビルボード行列
	XMMATRIX matBillboard_ = XMMatrixIdentity();

	//ターゲットまでの距離
	float DebugTargetDistance = 20.0f;
	//加算用
	float DebugChangeRot = (float)PI;
	float DebugChangeRot2 = 0.5f;
	float DebugChangeDistance = 0.0f;

	//プレイヤー
	//ターゲットまでの距離
	float playerTargetDistance = 20.0f;
	//加算用
	float playerChangeRot = (float)PI;
	float playerChangeRot2 = 0.5f;
	float playerChangeDistance = 0.0f;
};
