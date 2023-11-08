/**
 * @file Camera.h
 * @brief カメラ ゲームで使うビュー行列を管理
 * @author Asuma Syota
 * @date 2023/4
 */

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
public://静的メンバ変数

	/// <summary>
	///入力セット
	/// </summary>
	static void SetInput(Input* input) { Camera::input = input; }

	/// <summary>
	///コントローラー入力セット
	/// </summary>
	static void SetDXInput(DXInput* dxInput) { Camera::dxInput = dxInput; }

	//コンストラクタ デストラクタ
	Camera();
	~Camera();

public:

	/// <summary>
	///インスタンス取得
	/// </summary>
	static Camera* GetInstance();

	/// <summary>
	///初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///更新
	/// </summary>
	void Update();

	/// <summary>
	///タイトルの更新
	/// </summary>
	void TitleUpdate(XMFLOAT3 playerPos, XMFLOAT3 playerRot,float timer);

	/// <summary>
	///デバッグの更新
	/// </summary>
	void DebugUpdate();

	/// <summary>
	///プレイヤー追尾の更新
	/// </summary>
	void UpdatePlayer(XMFLOAT3 playerPos, XMFLOAT3 playerRot);

	/// <summary>
	///クリア画面の更新
	/// </summary>
	void UpdateClear(XMFLOAT3 enemyPos,float timer);

	/// <summary>
	///ビルボード行列の更新
	/// </summary>
	void BillboardUpdate();

	/// <summary>
	///注視点セット
	/// </summary>
	void SetTarget(XMFLOAT3 pos);

	/// <summary>
	///視点座標セット
	/// </summary>
	void SetEye(XMFLOAT3 pos);

	/// <summary>
	///視点座標取得
	/// </summary>
	XMFLOAT3 GetEye() { return eye_; };

	/// <summary>
	///注視点座標取得
	/// </summary>
	XMFLOAT3 GetTarget() { return target_; };

	/// <summary>
	///天井座標取得
	/// </summary>
	XMFLOAT3 GetUp() { return up_; };

	/// <summary>
	///射影変換取得
	/// </summary>
	XMMATRIX GetMatProjection() { return matProjection_; };

	/// <summary>
	///ビュー変換行列取得
	/// </summary>
	XMMATRIX GetMatView() { return matView_; };

	/// <summary>
	///ビュープロジェクション取得
	/// </summary>
	XMMATRIX GetMatViewProjection() { return matView_ * matProjection_; };

	/// <summary>
	///ビルボード用ビュー変換行列取得
	/// </summary>
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
	XMFLOAT3 up_ = { 0, 100, 0 };
	//ビルボード行列
	XMMATRIX matBillboard_ = XMMatrixIdentity();

	//ターゲットまでの距離
	float DebugTargetDistance = 40.0f;
	//加算用
	float DebugChangeRot = (float)PI;
	float DebugChangeRot2 = 0.5f;
	float DebugChangeDistance = 0.0f;

	//プレイヤー
	//ターゲットまでの距離
	float playerTargetDistance = 50.0f;
	//加算用
	float playerChangeRot = (float)PI;
	float playerChangeRot2 = 0.5f;
	float playerChangeDistance = 0.0f;
	float playerAddRot = 0.0f;
	float rotSpeed = (float)PI * 1.5f / 180.0f;

	//シーン遷移用
	XMFLOAT3 originalPlayerPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 originalPlayerRot = { 0.0f,0.0f,0.0f };
};