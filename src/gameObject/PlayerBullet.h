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
public:
	//静的メンバ関数
	static void SetCamera(Camera* camera) { PlayerBullet::camera = camera; }
	static void SetInput(Input* input) { PlayerBullet::input = input; }
	static void SetDXInput(DXInput* dxInput) { PlayerBullet::dxInput = dxInput; }
	static void SetModel(FbxModel* model) { PlayerBullet::model = model; }

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//移動
	void Move();
	//ショットフラグが真ならば弾生成
	void CreateBullet();
	//弾を削除する処理
	void DeleteBullet();

	//セッター
	void SetSRV(ID3D12DescriptorHeap* SRV);
	//ショットするフラグを受け取る
	void SetShotFlag(bool shotFlag) {PlayerBullet::shotFlag = shotFlag; };
	//弾に必要な情報をセットする
	void SetBullet(XMFLOAT3 position, XMFLOAT3 velocity);
	//ヒットフラグをセットする
	void SetHitFlag(bool hitFlag, int num) { PlayerBullet::hitFlag[num] = hitFlag; };

	//ゲッター
	/*XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }*/
	//コライダーデータ
	JSONLoader::ColliderData GetColliderData(int num);
	//弾の数
	size_t GetBulletNum() { return object.size(); }
	//座標
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
	float destoryTime= 120.0f;

	//弾につける番号
	int number = 0;

	//ショットフラグ
	bool shotFlag = false;

	//スピード
	float posSpeed = 1.0f;
};

