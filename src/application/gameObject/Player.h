/**
 * @file Player.h
 * @brief ゲームオブジェクト プレイヤー
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"
#include "PlayerBullet.h"
#include "ThunderParticle.h"
#include "JSONLoader.h"
#include "Sprite.h"

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
		Wait,	//待機
		Run,	//走り
		Attack1,//攻撃1
		Attack2,//攻撃2
		Attack3,//攻撃3
		Down,	//被ダメージ時
	};

	enum Form	//プレイヤーのフォルム
	{
		Fire,	//炎
		Elec,	//電気
	};

	//メンバ関数
public:

	//インストラクタ コンストラクタ
	Player() {};
	~Player();

	//静的メンバ関数

	/// <summary>
	///カメラセット
	/// </summary>
	static void SetCamera(Camera* camera) { Player::camera = camera; }

	/// <summary>
	///入力セット
	/// </summary>
	static void SetInput(Input* input) { Player::input = input; }

	/// <summary>
	///コントローラー入力セット
	/// </summary>
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }

	/// <summary>
	///初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///更新
	/// </summary>
	void Update();

	/// <summary>
	///更新 タイトルシーン
	/// </summary>
	void UpdateTitle(float timer);

	/// <summary>
	///更新 コライダーデータ
	/// </summary>
	void UpdateCollider();

	/// <summary>
	///更新 オブジェクト
	/// </summary>
	void UpdateObject();

	/// <summary>
	///更新 オブジェクト
	/// </summary>
	void UpdateObject(Status status, FbxObject3D* object);

	/// <summary>
	///更新 弾 パーティクル
	/// </summary>
	void UpdateParticle();

	/// <summary>
	///更新 スプライト
	/// </summary>
	void UpdateSprite();

	/// <summary>
	///1フレーム前の座標取得
	/// </summary>
	void UpdateOldTransform();

	/// <summary>
	///描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///ライト視点描画
	/// </summary>
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///パーティクル描画
	/// </summary>
	void DrawParticle(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///スプライト描画
	/// </summary>
	void DrawSprite(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///挙動 タイトルシーン
	/// </summary>
	void TitleControl(float timer);

	/// <summary>
	///挙動 ゲームシーン
	/// </summary>
	void GameControl();

	/// <summary>
	///更新 フォルム
	/// </summary>
	void UpdateForm();

	/// <summary>
	///移動 ゲームシーン
	/// </summary>
	void GameMove();

	/// <summary>
	///移動 タイトルシーン
	/// </summary>
	void TitleMove(float timer);

	/// <summary>
	///更新 落下
	/// </summary>
	void UpdateGravity();

	/// <summary>
	///更新 ジャンプ
	/// </summary>
	void UpdateJump();

	/// <summary>
	///ステータスマネージャー
	/// </summary>
	void StatusManager();

	/// <summary>
	///更新 攻撃
	/// </summary>
	void UpdateAttack();

	/// <summary>
	///更新 被ダメージ時
	/// </summary>
	void UpdateDown();

	/// <summary>
	///更新 炎攻撃
	/// </summary>
	void UpdateBullet1();

	/// <summary>
	///弾セット
	/// </summary>
	void SetBullet(PlayerBullet* playerBullet) { Player::bullet1 = playerBullet; }

	/// <summary>
	///srvセット
	/// </summary>
	void SetSRV(ID3D12DescriptorHeap* SRV);

	/// <summary>
	///座標セット
	/// </summary>
	void SetPosition(XMFLOAT3 pos) { position = pos; };

	/// <summary>
	///敵の座標取得
	/// </summary>
	void SetEnemyPos(XMFLOAT3 enemyPos) { this->enemyPos = enemyPos; }

	/// <summary>
	///平面との判定
	/// </summary>
	void HitPlane();

	/// <summary>
	///プレイヤーの弾命中時
	/// </summary>
	void HitBullet1(int num) { bullet1->SetHitFlag(num); };

	/// <summary>
	///敵の弾命中時
	/// </summary>
	void HitEnemy();

	/// <summary>
	///リセット
	/// </summary>
	void Reset();

	/// <summary>
	///座標取得
	/// </summary>
	XMFLOAT3 GetPosition() { return position; }

	/// <summary>
	///角度取得
	/// </summary>
	XMFLOAT3 GetRotation0() { return rotation0; }

	/// <summary>
	///角度取得
	/// </summary>
	XMFLOAT3 GetRotation1() { return rotation1; }

	/// <summary>
	///スケール取得
	/// </summary>
	XMFLOAT3 GetScale() { return scale; }

	/// <summary>
	///コライダー取得
	/// </summary>
	JSONLoader::ColliderData GetColliderData() { return colliderData; }

	/// <summary>
	///弾のコライダー取得
	/// </summary>
	JSONLoader::ColliderData GetBullet1ColliderData(int num) { return bullet1->GetColliderData(num); }

	/// <summary>
	///弾のコライダー取得
	/// </summary>
	size_t GetBullet1Num() { return  bullet1->GetBulletNum(); }

	/// <summary>
	///プレイヤーが無敵状態か取得
	/// </summary>
	bool GetInvincibleFlag() { return invincibleFlag; }

	/// <summary>
	///死亡フラグ取得
	/// </summary>
	bool GetIsDead() { return isDead; }

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

	//弾 炎
	PlayerBullet* bullet1 = nullptr;

	//コライダーデータ
	JSONLoader::ColliderData colliderData;

	//待ってる状態のオブジェクト
	FbxObject3D* objectWait = nullptr;
	//待ってる状態のモデル
	FbxModel* modelWait = nullptr;

	//走ってる状態のオブジェクト
	FbxObject3D* objectRun = nullptr;
	//走ってる状態のモデル
	FbxModel* modelRun = nullptr;

	//攻撃1のモデル
	FbxObject3D* objectAttack1 = nullptr;
	//攻撃1のオブジェクト
	FbxModel* modelAttack1 = nullptr;

	//攻撃2のオブジェクト
	FbxObject3D* objectAttack2 = nullptr;
	//攻撃2のモデル
	FbxModel* modelAttack2 = nullptr;

	//被ダメージ時のオブジェクト
	FbxObject3D* objectDown = nullptr;
	//被ダメージ時のモデル
	FbxModel* modelDown = nullptr;
	//被ダメージアニメーションのフレーム数
	float frameDown = 138.0f;

	//標準用のスプライト
	Sprite* rockOnSprite = nullptr;
	//標準用スプライトの大きさ、座標
	XMFLOAT2 rockOnSpriteScale = { 96.0f,96.0f };
	XMFLOAT2 rockOnSpritePos = { 1280.0f / 2.0f - rockOnSpriteScale.x / 2.0f,220.0f};

	//HP用スプライト
	Sprite* hpSprite1[5];	//緑のHP
	Sprite* hpSprite2 = nullptr;	//赤いHP
	Sprite* hpSprite3 = nullptr;	//HPの枠
	//HP用スプライトの大きさ、座標
	XMFLOAT2 hpSpritePos = { 490.0f,660.0f };
	XMFLOAT2 hpSprite1Scale = { 32.0f * 1.5f,16.0f * 1.5f };
	XMFLOAT2 hpSprite2Scale = { 32.0f * 1.5f,16.0f * 1.5f };
	XMFLOAT2 hpSprite3Scale = { 196.0f * 1.5,28.0f * 1.5 };
	//HP枠の大きさ
	float hpFrameScale1 = 10.0f * 1.5f;	//外枠 左右
	float hpFrameScale2 = 4.0f * 1.5f;	//内側の枠
	float hpFrameScale3 = 6.0f * 1.5f;	//外枠 上下

	//変形行列
	//平行移動
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 oldPosition = { 0.0f,0.0f,0.0f };
	//回転
	XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	XMFLOAT3 oldRotation0 = { 0.0f,0.0f,0.0f };
	XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };
	XMFLOAT3 oldRotation1 = { 0.0f,0.0f,0.0f };
	//サイズ
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	//進行ベクトル
	XMFLOAT3 posVelocity = { 0.0f,0.0f,0.0f };
	XMFLOAT3 oldPosVelocity = { 0.0f,0.0f,0.0f };
	//角度ベクトル
	XMFLOAT3 rotVelocity = { 0.0f,0.0f,0.0f };
	XMFLOAT3 oldRotVelocity = { 0.0f,0.0f,0.0f };
	float debugPos[2] = {320.0f,120.0f};

	//当たり判定関連
	//接地フラグ
	bool groundFlag = false;

	//ステータス関連
	//プレイヤーのHP
	float MaxHP = 5;
	float HP = MaxHP;


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
	float rot0Speed = (float)PI * 1.5f / 180.0f;

	//攻撃関連
	//Attack1のタイマー
	float Attack1Time = 150.0f;
	float Attack1Timer = 0.0f;
	//Attack2のタイマー
	float Attack2Time = 145.0f;
	float Attack2Timer = 0.0f;
	//連続攻撃の入力フレーム
	float Attack1IntervalTime = 50.0f;

	//状態
	Status status = Wait;
	//1フレーム前の状態
	Status preStatus = Wait;

	//フォルム
	Form form = Fire;
	Form preForm = Fire;
	//フォルムチェンジに使うタイマー
	int formTimer = 0;
	int formMaxTime = 120;
	//フォルムチェンジに使うフラグ
	bool formChangeFlag = false;

	//走ってるアニメーションに無理やりするフラグ
	bool runAnimationFlag = false;
	//シーン遷移に使う角度
	float addRot = 0.0f;
	float originalRot = 0.0f;

	//弾
	std::unique_ptr<ThunderParticle>thunderParticle;

	//敵の座標
	XMFLOAT3 enemyPos = { 0.0f,0.0f,0.0f };

	//炎の弾のタイマー
	int bullet1Timer = 0.0f;
	//炎を出す間隔
	int bullet1Interval = 10.0f;
	//炎のスケール
	XMFLOAT3  bullet1Scale = { 10.0f,10.0f,10.0f };
	XMFLOAT3  bullet1LastScale = { 1.0f,1.0f,1.0f };
	//炎を発生させる位置
	XMFLOAT3 bullet1AddPosition = { 0.0f,10.0f,0.0f };
	//炎を消すフレーム
	float bullet1Frame = 120.0f;

	//当たりフラグ
	bool hitFlag = false;
	float hitTimer = 0.0f;
	//被ダメージ時の無敵時間
	bool invincibleFlag = false;

	//死亡フラグ
	bool isDead = false;
};
