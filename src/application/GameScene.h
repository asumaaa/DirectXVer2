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
#include "ColliderFrustumObject.h"
#include "ColliderManager.h"
#include "SparkParticle.h"
#include "SparkParticle2.h"
#include "ExplosionParticle1.h"
#include "ExplosionParticle2.h"
#include "BillboardSprite.h"
#include "BillboardSpriteModel.h"
#include "ObjModel.h"
#include "ObjObject3D.h"
#include "DebugLineObject.h"
#include "DebugLineModel.h"
#include "EnemyBulletParticle.h"
#include "PlayerBulletParticle.h"
#include "ElecParticle.h"
#include "ElecParticle2.h"
#include "ElecObject.h"
#include "TutorialEnemy.h"
#include "Plane.h"

class GameScene
{
public:	//モード
	enum class Mode
	{
		Title,	//タイトル
		Tutorial,	//チュートリアル
		Game,	//ゲームシーン
		Clear,	//クリア
	};
	enum class ModeDraw
	{
		TitleDraw,	//タイトル
		TutorialDraw,	//チュートリアル
		GameDraw,	//ゲームシーン
		ClearDraw,	//クリア
	};
	enum class ModeDrawLightView
	{
		TitleDrawLightView,	//タイトル
		TutorialDrawLightView,	//チュートリアル
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
	void UpdateTutorial();	//チュートリアル
	void UpdateGame();	//ゲームシーン
	void UpdateClear();	//クリア
	//コライダーの更新
	void UpdateCollider();

	//描画
	void Draw();
	//モードごとの描画
	void DrawTitle();	//タイトル
	void DrawTutorial();	//チュートリアル
	void DrawGame();	//ゲームシーン
	void DrawClear();	//ゲームシーン
	//モードごとの描画
	//タイトル
	void DrawFBXTitle();			//FBX描画
	void DrawColliderTitle();		//コライダーの描画
	void DrawSpriteTitle();			//スプライト
	void DrawParticleTitle();		//パーティクル
	//チュートリアル
	void DrawFBXTutorial();			//FBX描画
	void DrawColliderTutorial();		//コライダーの描画
	void DrawSpriteTutorial();			//スプライト
	void DrawParticleTutorial();		//パーティクル
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
	void DrawFBXLightViewTutorial();	//チュートリアル
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
	size_t mode = static_cast<size_t>(Mode::Title);	//更新
	size_t modeDraw = static_cast<size_t>(ModeDraw::TitleDraw);		//描画
	size_t modeDrawLightView = static_cast<size_t>(ModeDrawLightView::TitleDrawLightView);//ライト目線描画

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
	bool TutorialFromGameFlag = false;

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

	//チュートリアルの敵
	std::unique_ptr<TutorialEnemy>tutorialEnemy;

	//平面
	std::unique_ptr<Plane> plane;

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
	std::unique_ptr<DebugLineModel>debugLineModel;
	std::unique_ptr<ColliderFrustumModel>colliderFrustumModel;

	//コライダーのオブジェクト
	std::unique_ptr<ColliderFrustumObject>colliderFrustumObject;

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

	//電気パーティクル
	std::unique_ptr<ElecParticle>elecParticle;

	//電気パーティクル
	std::unique_ptr<ElecParticle2>elecParticle2;

	//電気オブジェクト(デバッグ)
	std::unique_ptr<ElecObject>elecObject;

	//描画フラグ
	int drawParticle[1] = { 1 };
	int drawFbx[1] = { 1 };
	int drawSprite[1] = { 1 };
	int drawCollider[1] = { 0 };

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
	XMFLOAT2 game1Pos = { 900.0f, 600.0f };
	XMFLOAT2 game1Scale = { 227.0f, 101.0f };
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

	//スプライト
	//チュートリアル
	std::unique_ptr<Sprite>tutorial1Sprite;
	std::unique_ptr<Sprite>tutorial2Sprite;
	std::unique_ptr<Sprite>tutorial3Sprite;
	std::unique_ptr<Sprite>tutorial4Sprite;
	std::unique_ptr<Sprite>tutorial6Sprite;
	std::unique_ptr<Sprite>tutorial7Sprite;
	std::unique_ptr<Sprite>tutorial8Sprite;
	std::unique_ptr<Sprite>tutorial10Sprite;
	std::unique_ptr<Sprite>tutorial11Sprite;
	//チュートリアルのスプライト 座標 スケール
	XMFLOAT2 tutorial1Pos = { 128.0f, 520.0f };
	XMFLOAT2 tutorial1Scale = { 512.0f * 2, 130.0f * 1.5 };
	//チュートリアルのスプライト2 座標 スケール
	XMFLOAT2 tutorial2Pos = { 198.0f, 570.0f };
	XMFLOAT2 tutorial2Scale = { 791.0f, 101.0f };
	//チュートリアルのスプライト3 座標 スケール
	XMFLOAT2 tutorial3Pos = { 198.0f, 570.0f };
	XMFLOAT2 tutorial3Scale = { 104.0f * 2.0f, 42.0f * 2.0f };
	//チュートリアルのスプライト4 座標 スケール
	XMFLOAT2 tutorial4Pos = { 198.0f, 570.0f };
	XMFLOAT2 tutorial4Scale = { 791.0f, 101.0f };
	//チュートリアルのスプライト6 座標 スケール
	XMFLOAT2 tutorial6Pos = { 198.0f, 570.0f };
	XMFLOAT2 tutorial6Scale = { 791.0f, 101.0f };
	//チュートリアルのスプライト7 座標 スケール
	XMFLOAT2 tutorial7Pos = { 198.0f, 570.0f };
	XMFLOAT2 tutorial7Scale = { 791.0f, 101.0f };
	//チュートリアルのスプライト8 座標 スケール
	XMFLOAT2 tutorial8Pos = { 198.0f, 570.0f };
	XMFLOAT2 tutorial8Scale = { 791.0f, 101.0f };
	//チュートリアルのスプライト10 座標 スケール
	XMFLOAT2 tutorial10Pos = { 198.0f, 570.0f };
	XMFLOAT2 tutorial10Scale = { 791.0f, 101.0f };
	//チュートリアルのスプライト11 座標 スケール
	XMFLOAT2 tutorial11Pos = { 198.0f, 570.0f };
	XMFLOAT2 tutorial11Scale = { 791.0f, 101.0f };

	//チュートリアル用
	float blackSpriteTimer = 0.0f;
	float blackSpriteMaxTime = 120.0f;
	int tutorialSpriteFlag = 1;	//その番号のスプライト描画
	int tutorialLStickTimer = 0.0f;
	int tutorialLStickMaxTime = 240.0f;
	int tutorialRStickTimer = 0.0f;
	int tutorialRStickMaxTime = 240.0f;
	int tutorialIikannjiTimer = 0.0f;
	int tutorialIikannjiMaxTime = 200.0f;
	int tutorialAttackTimer = 0.0f;
	int tutorialAttackMaxTime = 200.0f;
	int tutorialAttack2Timer = 0.0f;
	int tutorialAttack2MaxTime = 2.0f;
	int tutorial12Timer = 0.0f;
	int tutorial12MaxTime = 120.0f;
	int tutorial13Timer = 0.0f;
	int tutorial13MaxTime =320.0f;

	float stickTest[2] = { 0.0f,0.0f };

	//爆発用
	float explosionTime = 10.0f;
	float explosionTimer = explosionTime;


	//タイマー
	float gameTimer = 0.0f;
};