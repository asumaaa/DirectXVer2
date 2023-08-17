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
	//メンバ関数
public:
	//静的メンバ関数
	static void SetCamera(Camera* camera) { Player::camera = camera; }
	static void SetInput(Input* input) { Player::input = input; }
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }

	//初期化
	void Initialize();
	//更新
	void Update();
	void UpdateObject();
	void UpdateBullet();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//挙動関連
	//挙動全般
	void Move();
	//キー操作
	void KeyControl();
	//落下
	void UpdateGravity();
	//ジャンプ
	void UpdateJump();

	//攻撃系全般
	void UpdateAttack();

	//セッター
	void SetBullet(PlayerBullet* playerBullet) { Player::bullet = playerBullet; }
	void SetObject(FbxObject3D* object);
	void SetSRV(ID3D12DescriptorHeap* SRV);
	void HitPlane();

	//外部ファイルによる初期値読み込み
	void LoadConfig();

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
	PlayerBullet*bullet;

	//オブジェクト
	std::unique_ptr<FbxObject3D>object;

	//変形行列
	//平行移動
	XMFLOAT3 position = {0.0f,0.0f,0.0f};
	//回転
	XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	XMFLOAT3 rotation1 = {0.0f,0.0f,0.0f};
	//サイズ
	XMFLOAT3 scale = {1.0f,1.0f,1.0f};
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
	float posSpeed = 0.15f;
	float rotSpeed = (float)PI * 1.0f / 180.0f;
};

