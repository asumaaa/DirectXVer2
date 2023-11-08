/**
 * @file GameScene.h
 * @brief ゲームを管理
 * @author Asuma Syota
 * @date 2023/4
 */

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
#include "ThunderParticle.h"
#include "ExplosionParticle1.h"
#include "ExplosionParticle2.h"
#include "ThunderParticle.h"
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
		Clear,	//クリア
	};
	enum class ModeDraw
	{
		TitleDraw,	//タイトル
		GameDraw,	//ゲームシーン
		ClearDraw,	//クリア
	};
	enum class ModeDrawLightView
	{
		TitleDrawLightView,	//タイトル
		GameDrawLightView,	//ゲームシーン
		ClearDrawLightView,	//クリア
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
	void UpdateClear();	//クリア
	//コライダーの更新
	void UpdateCollider();

	//描画
	void Draw();
	//モードごとの描画
	void DrawTitle();	//タイトル
	void DrawGame();	//ゲームシーン
	void DrawClear();	//ゲームシーン
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
	//クリア
	void DrawFBXClear();				//FBX描画
	void DrawColliderClear();		//コライダーの描画
	void DrawSpriteClear();			//スプライト
	void DrawParticleClear();		//パーティクル

	//ライト目線の描画
	void DrawFBXLightView();
	//モードごと
	void DrawFBXLightViewTitle();	//タイトル
	void DrawFBXLightViewGame();	//ゲーム
	void DrawFBXLightViewClear();	//クリア

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

	//シーン遷移
	float titleFromGameTime = 120.0f;
	float titleFromGameTimer = 0.0f;
	bool titleFromGameFlag = false;
	float gameFromTitleTime = 300.0f;
	float gameFromTitleTimer = 0.0f;
	bool gameFromTitleFlag = false;
	bool clearFromGameFlag = false;
	float clearFromGameTimer = 0.0f;
	float clearFromGameTime = 500.0f;
	bool gameFromClearFlag = false;

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
	std::unique_ptr<Enemy>enemy;

	//平面
	/*std::unique_ptr<Plane> plane;*/

	//天球
	std::unique_ptr<ObjModel>skySphereModel;
	std::unique_ptr<ObjObject3D>skySphereObject;
	//天球の座標 スケール
	XMFLOAT3 skySphereObjectPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 skySphereObjectRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 skySphereObjectScale = { 30.0f, 30.0f, 30.0f };

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
	//タイトル
	std::unique_ptr<Sprite>title1Sprite;
	std::unique_ptr<Sprite>title2Sprite;
	//タイトルスプライト 座標 スケール
	XMFLOAT2 title1Pos = { 0.0f,-150.0f };
	XMFLOAT2 title1Scale = { 1280.0f, 480.0f };
	XMFLOAT2 title2Pos = { 300.0f, 500.0f };
	XMFLOAT2 title2Scale = { 609.0f, 52.0f };
	//タイトルスプライト シーン遷移用
	int titleMoveTime = 120.0f;
	//ゲームシーン用スプライト
	std::unique_ptr<Sprite>game1Sprite;
	//ゲームシーン用スプライト 座標 スケール
	XMFLOAT2 game1Pos = { 0.0f, 0.0f };
	XMFLOAT2 game1Scale = { 399.0f, 60.0f };
	//黒いスプライト
	std::unique_ptr<Sprite>blackSprite1;
	std::unique_ptr<Sprite>blackSprite2;
	//黒いスプライト 座標 スケール
	XMFLOAT2 black1Pos = { 0.0f, 0.0f };
	XMFLOAT2 black1Scale = { 1280.0f, 720.0f };
	XMFLOAT2 black2Pos = { 0.0f, 0.0f };
	XMFLOAT2 black2Scale = { 1280.0f, 720.0f };
	//クリアシーン用スプライト
	std::unique_ptr<Sprite>clear1Sprite;
	//クリアシーン用スプライト 座標 スケール
	XMFLOAT2 clear1Pos = { 150.0f, 300.0f };
	XMFLOAT2 clear1Scale = { 860.0f, 77.0f };

	float stickTest[2] = { 0.0f,0.0f };

	//爆発用
	float explosionTime = 10.0f;
	float explosionTimer = explosionTime;
};