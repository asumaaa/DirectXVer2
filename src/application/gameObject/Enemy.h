/**
 * @file Enemy.h
 * @brief ゲームオブジェクト 敵
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"
#include "Sprite.h"

class Enemy
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	enum Status	//敵の状態
	{
		Stand,
		Walk,
		Attack1,
	};

public://メンバ関数

	/// <summary>
	///カメラセット
	/// </summary>
	static void SetCamera(Camera* camera) { Enemy::camera = camera; }

	/// <summary>
	///入力セット
	/// </summary>
	static void SetInput(Input* input) { Enemy::input = input; }

	/// <summary>
	///デバイスセット
	/// </summary>
	static void SetDXInput(DXInput* dxInput) { Enemy::dxInput = dxInput; }

	/// <summary>
	/// インストラクタ デストラクタ
	/// </summary>
	Enemy() {};
	~Enemy();

	/// <summary>
	///初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///更新
	/// </summary>
	void Update();

	/// <summary>
	///オブジェクト更新
	/// </summary>
	void UpdateObject();

	/// <summary>
	///スプライト更新
	/// </summary>
	void UpdateSprite();

	/// <summary>
	///描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///ライト視点描画
	/// </summary>
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///スプライト描画
	/// </summary>
	void DrawSprite(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///挙動全般
	/// </summary>
	void Move();

	/// <summary>
	///落下
	/// </summary>
	void UpdateGravity();

	/// <summary>
	///ジャンプ
	/// </summary>
	void UpdateJump();

	/// <summary>
	///攻撃系全般
	/// </summary>
	void UpdateAttack();

	/// <summary>
	///パターン制御
	/// </summary>
	void UpdateAttackPattern();

	/// <summary>
	///srvセット
	/// </summary>
	void SetSRV(ID3D12DescriptorHeap* SRV);

	/// <summary>
	///地面との判定
	/// </summary>
	void HitPlane();

	/// <summary>
	///座標取得
	/// </summary>
	XMFLOAT3 GetPosition() { return position; }

	/// <summary>
	///角度取得
	/// </summary>
	XMFLOAT3 GetRotation() { return rotation; }

	/// <summary>
	///スケール取得
	/// </summary>
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

	//立っている状態のオブジェクト
	FbxObject3D* objectStand = nullptr;
	//立っている状態のモデル
	FbxModel* modelStand = nullptr;
	//スプライト
	std::unique_ptr<Sprite>spriteHpBar;

	//変形行列
	//平行移動
	XMFLOAT3 position = { 0.0f,0.0f,30.0f };
	//回転
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	//サイズ
	XMFLOAT3 scale = { 5.0f,5.0f,5.0f };


	//当たり判定関連
	//接地フラグ
	bool groundFlag = false;


	//挙動関連

	//落下
	//落下ベクトル
	XMFLOAT3 fallVelocity = { 0.0f,0.0f,0.0f };
	//落下タイマー
	float fallTimer = 0.0f;
	//落下最大値までにかかる時間
	float fallTime = 1.0f;
	//1フレームあたりの落下量
	float fallFrame = 1.0f / 60.0f;

	//ジャンプ
	float jumpHeight = 0.4;

	//スピード
	float speed = 0.15f;

	//HP
	float HP = 100;
};
