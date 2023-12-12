/**
 * @file EnemyBullet.h
 * @brief ゲームオブジェクト 敵の弾
 * @author Asuma Syota
 * @date 2023/12
 */

#pragma once
#include "DirectXMath.h"
#include "Camera.h"
#include "JSONLoader.h"
#include "EnemyBulletParticle.h"

class EnemyBullet
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:	//サブクラス
	struct Bullet
	{
		//座標
		XMFLOAT3 position1;
		//座標(保存用)
		XMFLOAT3 position2;
		//座標(保存用)
		XMFLOAT3 position3;
		//回転
		XMFLOAT3 rotation;
		//最初のスケール
		XMFLOAT3 scale;
		//消滅時のスケール
		XMFLOAT3 lastScale;
		//進行ベクトル
		XMFLOAT3 velocity;
		//タイマー
		float timer;
		//弾が消える速度
		float destoryTime1;
		float destoryTime2;
		//フラグ
		bool hitFlag;
		//コライダーのデータ
		JSONLoader::ColliderData colliderData;
	};

public://静的メンバ関数

	/// <summary>
	///カメラセット
	/// </summary>
	static void SetCamera(Camera* camera) { EnemyBullet::camera = camera; }

public:

	//メンバ関数

	/// <summary>
	///初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///更新
	/// </summary>
	void Update();

	/// <summary>
	///コライダー更新
	/// </summary>
	void UpdateCollider();

	/// <summary>
	///パーティクル更新
	/// </summary>
	void UpdateParticle();

	/// <summary>
	///描画
	/// </summary>
	void Draw();

	/// <summary>
	///パーティクル描画
	/// </summary>
	void DrawParticle(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///移動
	/// </summary>
	void Move();

	/// <summary>
	///ショットフラグが真ならば弾生成
	/// </summary>
	void CreateBullet();

	/// <summary>
	///弾を削除する処理
	/// </summary>
	void DeleteBullet();

	/// <summary>
	///ショットするフラグを受け取る
	/// </summary>
	void SetShotFlag(bool shotFlag) { EnemyBullet::shotFlag = shotFlag; };

	/// <summary>
	///弾に必要な情報をセットする
	/// </summary>
	void SetBullet(XMFLOAT3 position,XMFLOAT3 scale,XMFLOAT3 lastScale,float timer,float destoryTime1, float destoryTime2 );

	/// <summary>
	///ヒットフラグをセットする
	/// </summary>
	void SetHitFlag(bool hitFlag, int num) { bullet[num].hitFlag = hitFlag; };

	/// <summary>
	///プレイヤーの座標セット
	/// </summary>
	void SetPlayerPos(XMFLOAT3 playerPos) { EnemyBullet::playerPos = playerPos; };

	/// <summary>
	///コライダーデータ取得
	/// </summary>
	JSONLoader::ColliderData GetColliderData(int num) { return bullet[num].colliderData; }

	/// <summary>
	///弾の数取得
	/// </summary>
	size_t GetBulletNum() { return bullet.size(); }

	/// <summary>
	///座標取得
	/// </summary>
	XMFLOAT3 GetPosition(int num) { return bullet[num].position1; }

	//静的メンバ変数
private:
	//カメラ
	static Camera* camera;

	//メンバ変数
private:

	//オブジェクト
	/*std::list<std::unique_ptr<FbxObject3D>>object;*/

	////座標
	//std::vector<XMFLOAT3> position;
	////回転
	//std::vector<XMFLOAT3> rotation;
	////サイズ
	//std::vector<XMFLOAT3> scale;
	////進行ベクトル
	//std::vector<XMFLOAT3> velocity;
	////タイマー
	//std::vector<float>timer;
	////フラグ
	//std::vector<bool>hitFlag;
	//パーティクル
	std::unique_ptr<EnemyBulletParticle>particle;
	//弾の情報
	std::vector<Bullet> bullet;

	XMFLOAT3 baseRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 baseScale = { 0.3f,0.3f,0.3f };
	//弾が消える速度
	/*float destoryTime = 120.0f;*/

	//弾につける番号
	int number = 0;
	//ショットフラグ
	bool shotFlag = false;

	//スピード
	float speed = 1.5f;

	//デバッグ用
	float bulletPos[3] = { 0.0f,0.0f,0.0f };

	//プレイヤーの座標
	XMFLOAT3 playerPos = { 0.0f,0.0f,0.0f };
};

