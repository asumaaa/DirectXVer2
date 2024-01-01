/**
 * @file PlayerBullet.h
 * @brief ゲームオブジェクト 自機の弾
 * @author Asuma Syota
 * @date 2023/12
 */

#pragma once
#include "DirectXMath.h"
#include "Camera.h"
#include "JSONLoader.h"
#include "PlayerBulletParticle.h"

class PlayerBullet
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
		//最初のスケール
		XMFLOAT3 scale;
		//進行ベクトル
		XMFLOAT3 velocity;
		//タイマー
		float timer;
		//弾が消える速度
		float destoryTime;
		//フラグ
		bool hitFlag;
		//コライダーのデータ
		JSONLoader::ColliderData colliderData;
	};

public://静的メンバ関数

	/// <summary>
	///カメラセット
	/// </summary>
	static void SetCamera(Camera* camera) { PlayerBullet::camera = camera; }

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
	void SetShotFlag(bool shotFlag) { PlayerBullet::shotFlag = shotFlag; };

	/// <summary>
	///弾に必要な情報をセットする
	/// </summary>
	void SetBullet(XMFLOAT3 position,XMFLOAT3 velocity,float timer,float destoryTime);

	/// <summary>
	///ヒットフラグをセットする
	/// </summary>
	//void SetHitFlag(bool hitFlag, int num) { bullet[num].hitFlag = hitFlag; };
	void SetHitFlag(int num);

	/// <summary>
	///敵の座標セット
	/// </summary>
	void SetEnemyPos(XMFLOAT3 enemyPos) { PlayerBullet::enemyPos = enemyPos; };

	/// <summary>
	///コライダーデータ取得
	/// </summary>
	//JSONLoader::ColliderData GetColliderData(int num) { return bullet[num].colliderData; }
	JSONLoader::ColliderData GetColliderData(int num);

	/// <summary>
	///弾の数取得
	/// </summary>
	size_t GetBulletNum() { return std::distance(bullet.begin(), bullet.end()); }

	/// <summary>
	///座標取得
	/// </summary>
	//XMFLOAT3 GetPosition(int num) { return bullet[num].position1; }
	XMFLOAT3 GetPosition(int num);

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
	std::unique_ptr<PlayerBulletParticle>particle;
	//弾の情報
	std::forward_list<Bullet> bullet;

	XMFLOAT3 baseRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 baseScale = { 1.0f,1.0f,1.0f };
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

	//敵の座標
	XMFLOAT3 enemyPos = { 0.0f,0.0f,0.0f };

	//弾の大きくなる拡大率
	float scaleAcc = 0.05f;
};

