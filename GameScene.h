#pragma once

#include "memory"
#include "list"

#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "FireParticle.h"
#include "Light.h"
#include "LightGroup.h"
#include "CSVLoader.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "ColliderCubeObject.h"
#include "ColliderSphereObject.h"
#include "ColliderPlaneObject.h"
#include "ColliderManager.h"
#include "SparkParticle.h"
#include "SparkParticle2.h"
#include "ExplosionParticle1.h"
#include "ExplosionParticle2.h"
#include "BillboardSprite.h"
#include "BillboardSpriteModel.h"
#include "ObjModel.h"
#include "ObjObject3D.h"

class GameScene
{
public:	//モード
	enum class Mode
	{
		Title,	//タイトル
		Game,	//ゲームシーン
	};
	enum class ModeDraw
	{
		TitleDraw,	//タイトル
		GameDraw,	//ゲームシーン
	};
	enum class ModeDrawLightView
	{
		TitleDrawLightView,	//タイトル
		GameDrawLightView,	//ゲームシーン
	};

	//メンバ関数
public:
	GameScene();
	~GameScene();
	//初期化
	void Initialize(DirectXCommon* dxCommon, Input* input, DXInput* dxInput);
	//終了時
	void Finalize();

	//更新
	void Update();
	//モードごとの更新
	void UpdateTitle();	//タイトル
	void UpdateGame();	//ゲームシーン
	//コライダーの更新
	void UpdateCollider();

	//描画
	void Draw();
	//モードごとの描画
	void DrawTitle();	//タイトル
	void DrawGame();	//ゲームシーン
	//モードごとの描画
	//タイトル
	void DrawFBXTitle();			//FBX描画
	void DrawColliderTitle();		//コライダーの描画
	void DrawSpriteTitle();			//スプライト
	void DrawParticleTitle();		//パーティクル
	//ゲーム
	void DrawFBXGame();				//FBX描画
	void DrawColliderGame();		//コライダーの描画
	void DrawSpriteGame();			//スプライト
	void DrawParticleGame();		//パーティクル

	//ライト目線の描画
	void DrawFBXLightView();
	//モードごと
	void DrawFBXLightViewTitle();	//タイトル
	void DrawFBXLightViewGame();	//ゲーム

	//モードマネージャー
	void ModeManager();

	//メンバ関数のポインタテーブル
	static void (GameScene::* Mode[])();
	static void (GameScene::* ModeDraw[])();
	static void (GameScene::* ModeDrawLightView[])();

	//セッター
	void SetSRV(ID3D12DescriptorHeap* SRV);
	//ゲッター
	DirectX::XMMATRIX GetLightViewProjection();

	//メンバ変数
private:
	//デバイスとinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = nullptr;
	//カメラ
	std::unique_ptr<Camera> camera_;

	//モード
	size_t mode = static_cast<size_t>(Mode::Game);	//更新
	size_t modeDraw = static_cast<size_t>(ModeDraw::GameDraw);		//描画
	size_t modeDrawLightView = static_cast<size_t>(ModeDrawLightView::GameDrawLightView);//ライト目線描画

	//fbx
	std::list<std::unique_ptr<FbxModel>> models;

	//レベルエディタ
	std::unique_ptr<JSONLoader> jsonLoader;

	//オブジェクト
	std::list<std::unique_ptr<FbxObject3D>> object;

	//ライト 影用
	std::unique_ptr<Light> light;
	float lightDir[3] = { 0.0f,-1.0f , -1.0f };
	float lightPos[3] = { 0.0f,25.0f,25.0f };
	float lightTarget[3] = { 0.0f,0.0f,0.0f };
	float lightFactorAngle[2] = { 20.0f,30.0f, };
	float lightAtten[3] = { 0.0f,0.0f,0.0f };

	//ライト
	std::unique_ptr<LightGroup> lightGroup;
	float lightManagerDir[3] = { 0.0f,-1.0f , 1.0f };

	//テクスチャマネージャー
	std::unique_ptr <TextureManager> textureManager;

	//変形行列
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };

	//プレイヤー
	std::unique_ptr<Player> player;
	////プレイヤーの弾
	//std::unique_ptr<PlayerBullet>playerBullet;

	////敵
	//std::unique_ptr<Enemy>enemy;

	//平面
	/*std::unique_ptr<Plane> plane;*/

	//天球
	std::unique_ptr<ObjModel>skySphereModel;
	std::unique_ptr<ObjObject3D>skySphereObject;

	//コライダーのモデル
	std::unique_ptr<ColliderCubeModel>colliderCubeModel;
	std::unique_ptr<ColliderSphereModel>colliderSphereModel;
	std::unique_ptr<ColliderPlaneModel>colliderPlaneModel;

	//コライダー
	std::unique_ptr<ColliderManager> colliderManager;

	//弾けるパーティクル
	std::unique_ptr<SparkParticle>sparkParticle;

	//弾けるパーティクル2
	std::unique_ptr<SparkParticle2>sparkParticle2;

	//爆発パーテイクル1
	std::unique_ptr<ExplosionParticle1>explosionParticle1;

	//爆発パーテイクル2
	std::unique_ptr<ExplosionParticle2>explosionParticle2;

	//描画フラグ
	int drawParticle[1] = { 1 };
	int drawFbx[1] = { 1 };
	int drawSprite[1] = { 1 };
	int drawCollider[1] = { 1 };

	//ビルボード　
	std::unique_ptr<BillboardSprite>billboardSprite;
	std::unique_ptr<BillboardSpriteModel>billboardSpriteModel;

	//スプライト
	std::unique_ptr<Sprite>title1Sprite;
	std::unique_ptr<Sprite>title2Sprite;
	std::unique_ptr<Sprite>game1Sprite;

	float stickTest[2] = { 0.0f,0.0f };
};