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
	//メンバ関数
public:
	//静的メンバ関数
	static void SetCamera(Camera* camera) { Enemy::camera = camera; }
	static void SetInput(Input* input) { Enemy::input = input; }
	static void SetDXInput(DXInput* dxInput) { Enemy::dxInput = dxInput; }

	//初期化
	void Initialize();
	//更新
	void Update();
	void UpdateObject();
	void UpdateSprite();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);
	void DrawSprite(ID3D12GraphicsCommandList* cmdList);

	//挙動関連
	//挙動全般
	void Move();
	//落下
	void UpdateGravity();
	//ジャンプ
	void UpdateJump();

	//攻撃系全般
	void UpdateAttack();

	//セッター
	void SetObject(FbxObject3D* object);
	void SetSRV(ID3D12DescriptorHeap* SRV);
	void HitPlane();

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
	std::unique_ptr<FbxObject3D>object;
	//スプライト
	std::unique_ptr<Sprite>spriteHpBar;

	//変形行列
	//平行移動
	XMFLOAT3 position = {0.0f,0.0f,0.0f};
	//回転
	XMFLOAT3 rotation = {0.0f,0.0f,0.0f};
	//サイズ
	XMFLOAT3 scale = {1.0f,1.0f,1.0f};


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

