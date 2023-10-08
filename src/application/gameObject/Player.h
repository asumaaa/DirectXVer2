#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"
#include "PlayerBullet.h"

class Player
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
	enum Status	//プレイヤーの状態
	{
		Wait,
		Run,
		BackRun,
		RunLeft,
		RunRight,
		Attack1,
		Attack2,
		Attack3,
	};
	//メンバ関数
public:

	Player() {};
	~Player();

	//静的メンバ関数
	static void SetCamera(Camera* camera) { Player::camera = camera; }
	static void SetInput(Input* input) { Player::input = input; }
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }

	//初期化
	void Initialize();
	//更新
	void Update();
	void UpdateObject();
	void UpdateObject(Status status, FbxObject3D* object);
	void UpdateBullet();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//挙動関連
	//挙動全般
	void Control();
	//移動
	void Move();
	//落下
	void UpdateGravity();
	//ジャンプ
	void UpdateJump();

	//ステータスマネージャー
	void StatusManager();

	//攻撃系全般
	void UpdateAttack();

	//セッター
	void SetBullet(PlayerBullet* playerBullet) { Player::bullet = playerBullet; }
	void SetSRV(ID3D12DescriptorHeap* SRV);
	void HitPlane();

	//ゲッター
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation0() { return rotation0; }
	XMFLOAT3 GetRotation1() { return rotation1; }
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

	//弾
	PlayerBullet* bullet;

	//待ってる状態のオブジェクト
	FbxObject3D* objectWait = nullptr;
	//待ってる状態のモデル
	FbxModel* modelWait = nullptr;

	//走ってる状態のオブジェクト
	FbxObject3D* objectRun = nullptr;
	//走ってる状態のモデル
	FbxModel* modelRun = nullptr;

	//後ろ走りしているプレイヤー
	FbxObject3D* objectBackRun = nullptr;
	//後ろ走りしてるモデル
	FbxModel* modelBackRun = nullptr;

	//左に走ってるオブジェクト
	FbxObject3D* objectRunLeft = nullptr;
	//左に走っているモデル
	FbxModel* modelRunLeft = nullptr;

	//右に走ってるオブジェクト
	FbxObject3D* objectRunRight = nullptr;
	//右に走っているモデル
	FbxModel* modelRunRight = nullptr;

	//攻撃1のモデル
	FbxObject3D* objectAttack1 = nullptr;
	//攻撃1のオブジェクト
	FbxModel* modelAttack1 = nullptr;

	//攻撃2のオブジェクト
	FbxObject3D* objectAttack2 = nullptr;
	//攻撃2のモデル
	FbxModel* modelAttack2 = nullptr;

	//変形行列
	//平行移動
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	//回転
	XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };
	//サイズ
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	//進行ベクトル
	XMFLOAT3 posVelocity = { 0.0f,0.0f,0.0f };
	//角度ベクトル
	XMFLOAT3 rotVelocity = { 0.0f,0.0f,0.0f };

	//当たり判定関連
	//接地フラグ
	bool groundFlag = false;

	//ステータス関連
	//プレイヤーのHP
	float HP = 10;


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
	float posSpeed = 1.0f;
	float rotSpeed = (float)PI * 1.5f / 180.0f;

	//攻撃関連
	//Attack1のタイマー
	float Attack1Time = 150.0f;
	float Attack1Timer = 0.0f;
	//Attack2のタイマー
	float Attack2Time = 145.0f;
	float Attack2Timer = 0.0f;
	//連続攻撃の入力フレーム
	float Attack1IntervalTime = 20.0f;

	//状態
	Status status = Wait;
	//1フレーム前の状態
	Status preStatus = Wait;
};
