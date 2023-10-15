/**
 * @file PlayerBullet.h
 * @brief ゲームオブジェクト プレイヤーの弾
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"

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

public:

	//メンバ関数
public://静的メンバ関数

	/// <summary>
	///デバイスセット
	/// </summary>
	static void SetCamera(Camera* camera) { PlayerBullet::camera = camera; }

	/// <summary>
	///入力セット
	/// </summary>
	static void SetInput(Input* input) { PlayerBullet::input = input; }

	/// <summary>
	///コントローラー入力
	/// </summary>
	static void SetDXInput(DXInput* dxInput) { PlayerBullet::dxInput = dxInput; }

	/// <summary>
	///モデルセット
	/// </summary>
	static void SetModel(FbxModel* model) { PlayerBullet::model = model; }

	/// <summary>
	///初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///更新
	/// </summary>
	void Update();

	/// <summary>
	///描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///ライト視点での描画
	/// </summary>
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

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
	///srvセット
	/// </summary>
	void SetSRV(ID3D12DescriptorHeap* SRV);

	/// <summary>
	///ショットするフラグを受け取る
	/// </summary>
	void SetShotFlag(bool shotFlag) { PlayerBullet::shotFlag = shotFlag; };

	/// <summary>
	///弾に必要な情報をセットする
	/// </summary>
	void SetBullet(XMFLOAT3 position, XMFLOAT3 velocity);

	/// <summary>
	///ヒットフラグをセットする
	/// </summary>
	void SetHitFlag(bool hitFlag, int num) { PlayerBullet::hitFlag[num] = hitFlag; };

	/// <summary>
	///コライダーデータ取得
	/// </summary>
	JSONLoader::ColliderData GetColliderData(int num);

	/// <summary>
	///弾の数取得
	/// </summary>
	size_t GetBulletNum() { return object.size(); }

	/// <summary>
	///座標取得
	/// </summary>
	XMFLOAT3 GetPosition(int num) { return position[num]; }

	//静的メンバ変数
private:
	//カメラ
	static Camera* camera;
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	//モデル
	static FbxModel* model;

	//メンバ変数
public:

	//オブジェクト
	std::list<std::unique_ptr<FbxObject3D>>object;

	//座標
	std::vector<XMFLOAT3> position;
	//回転
	std::vector<XMFLOAT3> rotation;
	//サイズ
	std::vector<XMFLOAT3> scale;
	//進行ベクトル
	std::vector<XMFLOAT3> velocity;
	//タイマー
	std::vector<float>timer;
	//フラグ
	std::vector<bool>hitFlag;

	XMFLOAT3 baseRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 baseScale = { 0.3f,0.3f,0.3f };
	//弾が消える速度
	float destoryTime = 120.0f;

	//弾につける番号
	int number = 0;

	//ショットフラグ
	bool shotFlag = false;

	//スピード
	float posSpeed = 1.0f;
};
