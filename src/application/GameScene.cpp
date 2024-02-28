/**
 * @file GameScene.cpp
 * @brief ゲームを管理
 * @author Asuma Syota
 * @date 2023/4
 */

#include "GameScene.h"
#include "FbxLoader.h"
#include<fstream>
#include "sstream"
#include "stdio.h"
#include "string.h"
#include "vector"
#include "imgui.h"
#include "mathOriginal.h"
#define PI 3.1415

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, DXInput* dxInput)
{
	this->dxCommon_ = dxCommon;
	this->input_ = input;
	this->dxInput = dxInput;

	//テクスチャマネージャー
	TextureManager::SetDevice(dxCommon->GetDevice());
	TextureManager* newTextureManager = new TextureManager();
	newTextureManager->Initialize();
	newTextureManager->LoadFile(0, L"Resources/pictures/white1x1.png");
	newTextureManager->LoadFile(1, L"Resources/pictures/toriko2.png");
	newTextureManager->LoadFile(2, L"Resources/pictures/GourmetSpyzer.png");
	newTextureManager->LoadFile(3, L"Resources/pictures/orange.png");
	newTextureManager->LoadFile(4, L"Resources/pictures/purple.png");
	newTextureManager->LoadFile(5, L"Resources/pictures/effect1.png");
	newTextureManager->LoadFile(6, L"Resources/pictures/effect2.png");
	newTextureManager->LoadFile(7, L"Resources/pictures/effect3.png");
	newTextureManager->LoadFile(8, L"Resources/pictures/enemyHP.png");
	newTextureManager->LoadFile(9, L"Resources/pictures/toriko.png");
	newTextureManager->LoadFile(10, L"Resources/pictures/grassFiled.png");
	newTextureManager->LoadFile(11, L"Resources/pictures/gravel.png");
	newTextureManager->LoadFile(12, L"Resources/pictures/DissolveMap.png");
	newTextureManager->LoadFile(13, L"Resources/pictures/mapping.png");
	newTextureManager->LoadFile(14, L"Resources/pictures/black.png");
	newTextureManager->LoadFile(15, L"Resources/pictures/blackParticle.png");
	newTextureManager->LoadFile(16, L"Resources/pictures/effect4.png");
	newTextureManager->LoadFile(17, L"Resources/pictures/skyBox.png");
	newTextureManager->LoadFile(18, L"Resources/pictures/title1.png");
	newTextureManager->LoadFile(19, L"Resources/pictures/title2.png");
	newTextureManager->LoadFile(20, L"Resources/pictures/game2.png");
	newTextureManager->LoadFile(21, L"Resources/pictures/clear.png");
	newTextureManager->LoadFile(22, L"Resources/pictures/rockOn.png");
	newTextureManager->LoadFile(23, L"Resources/pictures/enemyHP1.png");
	newTextureManager->LoadFile(24, L"Resources/pictures/enemyHP2.png");
	newTextureManager->LoadFile(25, L"Resources/pictures/enemyHP3.png");
	newTextureManager->LoadFile(26, L"Resources/pictures/enemyHP4.png");
	newTextureManager->LoadFile(27, L"Resources/pictures/fire.png");
	newTextureManager->LoadFile(28, L"Resources/pictures/playerHP1.png");
	newTextureManager->LoadFile(29, L"Resources/pictures/playerHP2.png");
	newTextureManager->LoadFile(30, L"Resources/pictures/playerHP3.png");
	newTextureManager->LoadFile(31, L"Resources/pictures/elec.jpeg");
	newTextureManager->LoadFile(32, L"Resources/pictures/tutorial1.png");
	newTextureManager->LoadFile(33, L"Resources/pictures/tutorial2.png");
	newTextureManager->LoadFile(34, L"Resources/pictures/tutorial3.png");
	newTextureManager->LoadFile(35, L"Resources/pictures/tutorial4.png");
	newTextureManager->LoadFile(36, L"Resources/pictures/tutorial6.png");
	newTextureManager->LoadFile(37, L"Resources/pictures/tutorial7.png");
	newTextureManager->LoadFile(38, L"Resources/pictures/tutorial8.png");
	newTextureManager->LoadFile(39, L"Resources/pictures/tutorial10.png");
	newTextureManager->LoadFile(40, L"Resources/pictures/tutorial11.png");
	newTextureManager->LoadFile(41, L"Resources/pictures/plane.png");

	textureManager.reset(newTextureManager);

	//FBXローダー初期化
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//テクスチャマネージャーセット
	FbxModel::SetTextureManager(textureManager.get());
	FbxModel::SetDevice(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("plane"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree1"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree2"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree3"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("cube"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("player"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("enemy"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("playerBullet"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("sphere"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("playerWait"));

	//スプライト
	Sprite::SetDevice(dxCommon->GetDevice());
	Sprite::SetTextureManager(textureManager.get());
	Sprite::CreateGraphicsPipeLine();
	//タイトルのスプライト
	Sprite* newTitle1Sprite = new Sprite();
	newTitle1Sprite->Initialize();
	title1Sprite.reset(newTitle1Sprite);
	title1Sprite->SetTextureNum(18);
	title1Sprite->SetPosition(title1Pos);
	title1Sprite->SetScale(title1Scale);
	//タイトルのスプライト2
	Sprite* newTitle2Sprite = new Sprite();
	newTitle2Sprite->Initialize();
	title2Sprite.reset(newTitle2Sprite);
	title2Sprite->SetTextureNum(19);
	title2Sprite->SetPosition(title2Pos);
	title2Sprite->SetScale(title2Scale);
	//ゲームのスプライト1
	Sprite* newGame1Sprite = new Sprite();
	newGame1Sprite->Initialize();
	game1Sprite.reset(newGame1Sprite);
	game1Sprite->SetTextureNum(20);
	game1Sprite->SetPosition(game1Pos);
	game1Sprite->SetScale(game1Scale);
	//黒いスプライト1
	Sprite* newBlackSprite1 = new Sprite();
	newBlackSprite1->Initialize();
	blackSprite1.reset(newBlackSprite1);
	blackSprite1->SetTextureNum(14);
	blackSprite1->SetPosition(black1Pos);
	blackSprite1->SetScale(black1Scale);
	//黒いスプライト2
	Sprite* newBlackSprite2 = new Sprite();
	newBlackSprite2->Initialize();
	blackSprite2.reset(newBlackSprite2);
	blackSprite2->SetTextureNum(14);
	blackSprite2->SetPosition(black2Pos);
	blackSprite2->SetScale(black2Scale);
	//クリアのスプライト1
	Sprite* newClear1Sprite = new Sprite();
	newClear1Sprite->Initialize();
	clear1Sprite.reset(newClear1Sprite);
	clear1Sprite->SetTextureNum(21);
	clear1Sprite->SetPosition(clear1Pos);
	clear1Sprite->SetScale(clear1Scale);
	//チュートリアルのスプライト1
	Sprite* newTutorial1Sprite = new Sprite();
	newTutorial1Sprite->Initialize();
	tutorial1Sprite.reset(newTutorial1Sprite);
	tutorial1Sprite->SetTextureNum(32);
	tutorial1Sprite->SetPosition(tutorial1Pos);
	tutorial1Sprite->SetScale(tutorial1Scale);
	//チュートリアルのスプライト2
	Sprite* newTutorial2Sprite = new Sprite();
	newTutorial2Sprite->Initialize();
	tutorial2Sprite.reset(newTutorial2Sprite);
	tutorial2Sprite->SetTextureNum(33);
	tutorial2Sprite->SetPosition(tutorial2Pos);
	tutorial2Sprite->SetScale(tutorial2Scale);
	//チュートリアルのスプライト3
	Sprite* newTutorial3Sprite = new Sprite();
	newTutorial3Sprite->Initialize();
	tutorial3Sprite.reset(newTutorial3Sprite);
	tutorial3Sprite->SetTextureNum(34);
	tutorial3Sprite->SetPosition(tutorial3Pos);
	tutorial3Sprite->SetScale(tutorial3Scale);
	//チュートリアルのスプライト4
	Sprite* newTutorial4Sprite = new Sprite();
	newTutorial4Sprite->Initialize();
	tutorial4Sprite.reset(newTutorial4Sprite);
	tutorial4Sprite->SetTextureNum(35);
	tutorial4Sprite->SetPosition(tutorial4Pos);
	tutorial4Sprite->SetScale(tutorial4Scale);
	//チュートリアルのスプライト6
	Sprite* newTutorial6Sprite = new Sprite();
	newTutorial6Sprite->Initialize();
	tutorial6Sprite.reset(newTutorial6Sprite);
	tutorial6Sprite->SetTextureNum(36);
	tutorial6Sprite->SetPosition(tutorial6Pos);
	tutorial6Sprite->SetScale(tutorial6Scale);
	//チュートリアルのスプライト7
	Sprite* newTutorial7Sprite = new Sprite();
	newTutorial7Sprite->Initialize();
	tutorial7Sprite.reset(newTutorial7Sprite);
	tutorial7Sprite->SetTextureNum(37);
	tutorial7Sprite->SetPosition(tutorial6Pos);
	tutorial7Sprite->SetScale(tutorial6Scale);
	//チュートリアルのスプライト8
	Sprite* newTutorial8Sprite = new Sprite();
	newTutorial8Sprite->Initialize();
	tutorial8Sprite.reset(newTutorial8Sprite);
	tutorial8Sprite->SetTextureNum(38);
	tutorial8Sprite->SetPosition(tutorial8Pos);
	tutorial8Sprite->SetScale(tutorial8Scale);
	//チュートリアルのスプライト10
	Sprite* newTutorial10Sprite = new Sprite();
	newTutorial10Sprite->Initialize();
	tutorial10Sprite.reset(newTutorial10Sprite);
	tutorial10Sprite->SetTextureNum(39);
	tutorial10Sprite->SetPosition(tutorial10Pos);
	tutorial10Sprite->SetScale(tutorial10Scale);
	//チュートリアルのスプライト11
	Sprite* newTutorial11Sprite = new Sprite();
	newTutorial11Sprite->Initialize();
	tutorial11Sprite.reset(newTutorial11Sprite);
	tutorial11Sprite->SetTextureNum(40);
	tutorial11Sprite->SetPosition(tutorial11Pos);
	tutorial11Sprite->SetScale(tutorial11Scale);

	//カメラ初期化
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	/*camera_->SetEye({ 0.0f,0.0f,0.0f });
	camera_->SetTarget({ 0.0f,0.0f,0.0f });*/

	//ライト(影)
	Light* newLight = new Light();
	newLight->Initialize();
	light.reset(newLight);

	//ライト
	LightGroup::StaticInitialize(dxCommon_->GetDevice());
	LightGroup* newLightGroup = new LightGroup();
	newLightGroup = LightGroup::Create();
	lightGroup.reset(newLightGroup);

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLight(light.get());
	FbxObject3D::SetLightGroup(lightGroup.get());
	FbxObject3D::CreateGraphicsPipelineLightView();
	FbxObject3D::CreateGraphicsPipeline();

	//コライダーのキューブ
	ColliderCubeModel* newCubeModel = new ColliderCubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice());
	colliderCubeModel.reset(newCubeModel);
	ColliderCubeObject::SetDevice(dxCommon_->GetDevice());
	ColliderCubeObject::SetCamera(camera_.get());
	ColliderCubeObject::SetInput(input_);
	ColliderCubeObject::SetModel(colliderCubeModel.get());
	ColliderCubeObject::CreateGraphicsPipeline();

	//コライダーの球
	ColliderSphereModel* newSphereModel = new ColliderSphereModel();
	newSphereModel->CreateBuffers(dxCommon_->GetDevice());
	colliderSphereModel.reset(newSphereModel);
	ColliderSphereObject::SetDevice(dxCommon_->GetDevice());
	ColliderSphereObject::SetCamera(camera_.get());
	ColliderSphereObject::SetInput(input_);
	ColliderSphereObject::SetModel(colliderSphereModel.get());
	ColliderSphereObject::CreateGraphicsPipeline();

	//コライダーの平面
	ColliderPlaneModel* newPlaneModel = new ColliderPlaneModel();
	newPlaneModel->CreateBuffers(dxCommon_->GetDevice());
	colliderPlaneModel.reset(newPlaneModel);
	ColliderPlaneObject::SetDevice(dxCommon_->GetDevice());
	ColliderPlaneObject::SetCamera(camera_.get());
	ColliderPlaneObject::SetInput(input_);
	ColliderPlaneObject::SetModel(colliderPlaneModel.get());
	ColliderPlaneObject::CreateGraphicsPipeline();

	//デバッグ用線分
	ColliderFrustumModel* newDebugFrustumModel = new ColliderFrustumModel();
	newDebugFrustumModel->CreateBuffers(dxCommon_->GetDevice());
	colliderFrustumModel.reset(newDebugFrustumModel);
	ColliderFrustumObject::SetDevice(dxCommon_->GetDevice());
	ColliderFrustumObject::SetCamera(camera_.get());
	ColliderFrustumObject::SetInput(input_);
	ColliderFrustumObject::SetModel(colliderFrustumModel.get());
	ColliderFrustumObject::CreateGraphicsPipeline();
	ColliderFrustumObject* newDebugFrustumObject = new ColliderFrustumObject();
	newDebugFrustumObject->Initialize();
	colliderFrustumObject.reset(newDebugFrustumObject);
	colliderFrustumObject->SetPosition(XMFLOAT3(0.0f, 5.0f, 0.0f));
	colliderFrustumObject->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	colliderFrustumObject->SetScale(XMFLOAT3(3.0f,3.0f,30.0f));

	//コライダーマネージャー
	ColliderManager::SetColliderCubeModel(colliderCubeModel.get());
	ColliderManager::SetColliderSphereModel(colliderSphereModel.get());
	ColliderManager* newColliderManager = new ColliderManager();
	colliderManager.reset(newColliderManager);

	//弾けるパーティクル
	SparkParticle::SetSpriteManager(textureManager.get());
	SparkParticle::SetDevice(dxCommon_->GetDevice());
	SparkParticle::SetCamera(camera_.get());
	SparkParticle::SetInput(input_);
	SparkParticle::CreateGraphicsPipeline();
	SparkParticle* newSparkParticle = new SparkParticle();
	newSparkParticle->CreateBuffers();
	newSparkParticle->SetTextureNum(5);
	sparkParticle.reset(newSparkParticle);

	//弾けるパーティクル
	SparkParticle2::SetSpriteManager(textureManager.get());
	SparkParticle2::SetDevice(dxCommon_->GetDevice());
	SparkParticle2::SetCamera(camera_.get());
	SparkParticle2::SetInput(input_);
	SparkParticle2::CreateGraphicsPipeline();
	SparkParticle2* newSparkParticle2 = new SparkParticle2();
	newSparkParticle2->CreateBuffers();
	newSparkParticle2->SetTextureNum(3);
	sparkParticle2.reset(newSparkParticle2);

	//爆発パーティクル
	ExplosionParticle1::SetSpriteManager(textureManager.get());
	ExplosionParticle1::SetDevice(dxCommon_->GetDevice());
	ExplosionParticle1::SetCamera(camera_.get());
	ExplosionParticle1::SetInput(input_);
	ExplosionParticle1::CreateGraphicsPipeline();
	ExplosionParticle1* newExplosionParticle1 = new ExplosionParticle1();
	newExplosionParticle1->CreateBuffers();
	newExplosionParticle1->SetTextureNum(16);
	explosionParticle1.reset(newExplosionParticle1);

	//爆発パーティクル
	ExplosionParticle2::SetSpriteManager(textureManager.get());
	ExplosionParticle2::SetDevice(dxCommon_->GetDevice());
	ExplosionParticle2::SetCamera(camera_.get());
	ExplosionParticle2::SetInput(input_);
	ExplosionParticle2::CreateGraphicsPipeline();
	ExplosionParticle2* newExplosionParticle2 = new ExplosionParticle2();
	newExplosionParticle2->CreateBuffers();
	newExplosionParticle2->SetTextureNum(16);
	explosionParticle2.reset(newExplosionParticle2);

	//敵の弾パーティクル
	EnemyBulletParticle::SetSpriteManager(textureManager.get());
	EnemyBulletParticle::SetDevice(dxCommon_->GetDevice());
	EnemyBulletParticle::SetCamera(camera_.get());
	EnemyBulletParticle::SetInput(input_);
	EnemyBulletParticle::CreateGraphicsPipeline();

	//自機の弾パーティクル
	PlayerBulletParticle::SetSpriteManager(textureManager.get());
	PlayerBulletParticle::SetDevice(dxCommon_->GetDevice());
	PlayerBulletParticle::SetCamera(camera_.get());
	PlayerBulletParticle::SetInput(input_);
	PlayerBulletParticle::CreateGraphicsPipeline();

	//電気パーティクル
	ElecParticle::SetSpriteManager(textureManager.get());
	ElecParticle::SetDevice(dxCommon_->GetDevice());
	ElecParticle::SetCamera(camera_.get());
	ElecParticle::SetInput(input_);
	ElecParticle::CreateGraphicsPipeline();
	ElecParticle* newElecParticle = new ElecParticle();
	newElecParticle->CreateBuffers();
	newElecParticle->SetTextureNum(4);
	elecParticle.reset(newElecParticle);

	//電気パーティクル
	ElecParticle2::SetSpriteManager(textureManager.get());
	ElecParticle2::SetDevice(dxCommon_->GetDevice());
	ElecParticle2::SetCamera(camera_.get());
	ElecParticle2::SetInput(input_);
	ElecParticle2::CreateGraphicsPipeline();
	ElecParticle2* newElecParticle2 = new ElecParticle2();
	newElecParticle2->CreateBuffers();
	newElecParticle2->SetTextureNum(4);
	elecParticle2.reset(newElecParticle2);

	//電気オブジェクト(デバッグ)
	ElecObject::SetDevice(dxCommon_->GetDevice());
	ElecObject::SetCamera(camera_.get());
	ElecObject::SetInput(input_);
	ElecObject::CreateGraphicsPipeline();
	ElecObject* newElecObject = new ElecObject();
	newElecObject->Initialize(XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),0.0f);
	elecObject.reset(newElecObject);

	//ビルボードのスプライト
	BillboardSpriteModel::SetDevice(dxCommon_->GetDevice());
	BillboardSpriteModel::SetSpriteManager(textureManager.get());
	BillboardSpriteModel* newBillboardSpriteModel = new BillboardSpriteModel();
	newBillboardSpriteModel->CreateBuffers(dxCommon_->GetDevice());
	newBillboardSpriteModel->SetTextureNum(8);
	billboardSpriteModel.reset(newBillboardSpriteModel);
	BillboardSprite::SetDevice(dxCommon_->GetDevice());
	BillboardSprite::SetCamera(camera_.get());
	BillboardSprite::SetInput(input_);
	BillboardSprite::SetModel(billboardSpriteModel.get());
	BillboardSprite::CreateGraphicsPipeline();
	BillboardSprite* newBillboardSprite = new BillboardSprite();
	newBillboardSprite->Initialize();
	billboardSprite.reset(newBillboardSprite);

	//天球
	ObjModel* newObjModel = new ObjModel();
	newObjModel->Initialize(dxCommon_->GetDevice(), "skyBox", "Resources/pictures/skyBox.png");
	skySphereModel.reset(newObjModel);
	ObjObject3D* newObject = new ObjObject3D();
	newObject->Initialize(dxCommon_->GetDevice(), newObjModel, camera_.get());
	skySphereObject.reset(newObject);
	skySphereObject->setScale(skySphereObjectScale);
	skySphereObject->setRotation(skySphereObjectRot);
	skySphereObject->setPosition(skySphereObjectPos);

	//プレイヤー
	Player::SetCamera(camera_.get());
	Player::SetInput(input);
	Player::SetDXInput(dxInput);
	Player* newPlayer = new Player();
	newPlayer->Initialize();
	player.reset(newPlayer);

	//敵
	Enemy::SetCamera(camera_.get());
	Enemy::SetInput(input);
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize();
	enemy.reset(newEnemy);
	//敵の弾
	EnemyBullet::SetCamera(camera_.get());

	//チュートリアルの敵
	TutorialEnemy::SetCamera(camera_.get());
	TutorialEnemy::SetInput(input);
	TutorialEnemy* newTutorialEnemy = new TutorialEnemy();
	newTutorialEnemy->Initialize();
	tutorialEnemy.reset(newTutorialEnemy);

	//平面
	Plane::SetCamera(camera_.get());
	Plane::SetInput(input);
	Plane* newPlane = new Plane();
	newPlane->Initialize();
	plane.reset(newPlane);

	//レベルエディタ
	JSONLoader* newJsonLoader = new JSONLoader();
	newJsonLoader->LoadFile("Resources/json/demo2.json");
	jsonLoader.reset(newJsonLoader);

	for (int i = 0; i < jsonLoader->GetObjectNum(); i++)
	{
		std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
		//オブジェクト初期化
		newObject->Initialize();

		//モデルセット
		for (std::unique_ptr<FbxModel>& model : models)
		{
			if (jsonLoader->GetFileName(i) == model->GetFileName())
			{
				newObject->SetModel(model.get());
			}
		}

		//オブジェクトの配置
		newObject->SetObjectData(jsonLoader->GetObjectData(i));
		//コライダーの配置
		newObject->SetColliderData(jsonLoader->GetColliderData(i));
		//テクスチャデータのセット
		newObject->SetTextureData(jsonLoader->GetTextureData(i));

		object.push_back(std::move(newObject));

		//コライダーのセット
		ColliderManager::SetCollider(jsonLoader->GetColliderData(i));
	}
}

void GameScene::Finalize()
{
	models.clear();
	delete textureManager.get();
	delete lightGroup.get();
	object.clear();
	/*delete tree1csv;
	delete spriteManager;
	delete lightGroup;*/
}

void GameScene::UpdateTitle()
{
	gameTimer = 0.0f;
	//カメラ更新
	/*camera_->DebugUpdate();*/
	camera_->TitleUpdate(player->GetPosition(), player->GetRotation0(), gameFromTitleTimer);
	camera_->Update();

	billboardSprite->SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
	billboardSprite->SetScale(XMFLOAT3(2.5f, 0.3f, 1.0f));
	billboardSprite->Update();

	//スプライト更新
	if (gameFromTitleTimer > titleMoveTime)
	{
		title1Sprite->SetPosition(XMFLOAT2(title1Pos.x, title1Pos.y - 
			easeOutCirc((gameFromTitleTimer - titleMoveTime) / 180) * 300.0f));
		title2Sprite->SetPosition(XMFLOAT2(title2Pos.x, title2Pos.y + 
			easeOutCirc((gameFromTitleTimer - titleMoveTime) / 180) * 300.0f));
	}
	else
	{
		title1Sprite->SetPosition(title1Pos);
		title2Sprite->SetPosition(title2Pos);
	}
	if (gameFromTitleTimer > 220)
	{
		blackSprite1->SetAlpha((gameFromTitleTimer - 220.0f) / 80.0f);
	}
	else
	{
		blackSprite1->SetAlpha(0.0f);
	}
	title1Sprite->Update();
	title2Sprite->Update();
	blackSprite1->Update();

	//ライト
	lightTarget[0] = player->GetPosition().x + 25;
	lightTarget[1] = player->GetPosition().y + 25;
	lightTarget[2] = player->GetPosition().z + 25;
	lightPos[0] = player->GetPosition().x;
	lightPos[1] = player->GetPosition().y;
	lightPos[2] = player->GetPosition().z;
	light->SetEye(XMFLOAT3(lightPos));
	light->SetTarget(XMFLOAT3(lightTarget));
	light->SetDir(XMFLOAT3(lightDir));
	light->Update();

	//ライト
	lightGroup->SetAmbientColor(XMFLOAT3(1, 1, 1));
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->Update();

	//天球
	if (gameFromTitleTimer < 119)
	{
		skySphereObject->HomingUpdate(player->GetPosition());
	}
	skySphereObject->Update();

	//プレイヤー
	player->UpdateTitle(gameFromTitleTimer);

	//オブジェクト更新
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Update();
	}

	//コライダー更新
	UpdateCollider();
}

void GameScene::UpdateTutorial()
{
	if (gameTimer == 0.0f)
	{
		player->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
	gameTimer += 1.0f;

	//カメラ更新
	if (tutorialSpriteFlag == 13)
	{
		camera_->UpdateTutorial(tutorial13Timer);
	}
	else
	{
		camera_->UpdatePlayer(player->GetPosition(), player->GetRotation0());
	}
	/*camera_->DebugUpdate();*/
	camera_->Update();

	//判定 デバッグ
	if (input_->PushKey(DIK_I))
	{
		camera_->SetDebugFlag(true);
	}
	else
	{
		camera_->SetDebugFlag(false);
	}

	//デバッグ用
	elecParticle->Add(XMFLOAT3(0.0f, 5.0f, 0.0f), 1.0f);
	elecObject->Update();
	//更新
	elecParticle->Update();
	if (input_->TriggerKey(DIK_N))
	{
		elecParticle2->Add(XMFLOAT3(10.0f, 5.0f, 0.0f), XMFLOAT3(-10.0f, 5.0f, 0.0f), 1.0f);
	}
	//更新
	elecParticle2->Update();

	//スプライトの処理
	//黒幕
	if (tutorialSpriteFlag == 1)
	{
		blackSpriteTimer++;
		blackSprite1->SetAlpha((blackSpriteMaxTime - blackSpriteTimer) / blackSpriteMaxTime);
		//時間経過でスティック操作チュートリアルへ
		if (blackSpriteTimer >= blackSpriteMaxTime)
		{
			tutorialSpriteFlag = 2;
			blackSpriteTimer = 0;
		}
	}
	//スティック操作チュートリアル
	if (tutorialSpriteFlag == 2)
	{
		//Rスティックの入力があればタイマー更新
		if (dxInput->GetStick(DXInput::RStickX) >= 0.4 || dxInput->GetStick(DXInput::RStickX) <= -0.4 ||
			dxInput->GetStick(DXInput::RStickY) >= 0.4 || dxInput->GetStick(DXInput::RStickY) <= -0.4)
		{
			tutorialRStickTimer++;
		}
		//Lスティックの入力があればタイマー更新
		if (dxInput->GetStick(DXInput::LStickX) >= 0.4 || dxInput->GetStick(DXInput::LStickX) <= -0.4 ||
			dxInput->GetStick(DXInput::LStickY) >= 0.4 || dxInput->GetStick(DXInput::LStickY) <= -0.4)
		{
			tutorialLStickTimer++;
		}
		//どっちも達成で次へ
		if (tutorialLStickTimer >= tutorialLStickMaxTime && tutorialRStickTimer >= tutorialRStickMaxTime)
		{
			tutorialSpriteFlag = 3;
			tutorialLStickTimer = 0;
			tutorialRStickTimer = 0;
		}
	}
	//いい感じチュートリアル
	if (tutorialSpriteFlag == 3)
	{
		tutorialIikannjiTimer++;
		//時間経過で攻撃チュートリアルへ
		if (tutorialIikannjiTimer >= tutorialIikannjiMaxTime)
		{
			tutorialSpriteFlag = 4;
			tutorialIikannjiTimer = 0;
		}
	}
	//攻撃チュートリアル
	if (tutorialSpriteFlag == 4)
	{
		if (dxInput->PushKey(DXInput::PAD_RIGHT_SHOULDER) == 1)
		{
			tutorialAttackTimer++;
		}
		//達成で次へ
		if (tutorialAttackTimer >= tutorialAttackMaxTime)
		{
			tutorialSpriteFlag = 5;
			tutorialAttackTimer = 0;
		}
	}
	//いい感じチュートリアル
	if (tutorialSpriteFlag == 5)
	{
		tutorialIikannjiTimer++;
		//時間経過で攻撃チュートリアルへ
		if (tutorialIikannjiTimer >= tutorialIikannjiMaxTime)
		{
			tutorialSpriteFlag = 6;
			tutorialIikannjiTimer = 0;
		}
	}
	//属性変化チュートリアル
	if (tutorialSpriteFlag == 6)
	{
		if (dxInput->PushKey(DXInput::PAD_LEFT_SHOULDER) == 1)
		{
			tutorialSpriteFlag = 7;
		}
	}
	//属性変化チュートリアル2
	if (tutorialSpriteFlag == 7)
	{
		tutorialIikannjiTimer++;
		//時間経過で攻撃チュートリアルへ
		if (tutorialIikannjiTimer >= tutorialIikannjiMaxTime)
		{
			tutorialSpriteFlag = 8;
			tutorialIikannjiTimer = 0;
		}
	}
	//攻撃チュートリアル
	if (tutorialSpriteFlag == 8)
	{
		if (dxInput->TriggerKey(DXInput::PAD_RIGHT_SHOULDER) == 1)
		{
			tutorialAttack2Timer++;
		}
		//達成で次へ
		if (tutorialAttack2Timer >= tutorialAttack2MaxTime)
		{
			tutorialSpriteFlag = 9;
			tutorialAttack2Timer = 0;
		}
	}
	//属性変化チュートリアル2
	if (tutorialSpriteFlag == 9)
	{
		tutorialIikannjiTimer++;
		//時間経過で攻撃チュートリアルへ
		if (tutorialIikannjiTimer >= tutorialIikannjiMaxTime)
		{
			tutorialSpriteFlag = 10;
			tutorialIikannjiTimer = 0;
		}
	}
	//敵を倒すチュートリアル
	if (tutorialSpriteFlag == 10)
	{
		tutorialEnemy->Update();
		//敵を倒したら次へ
		if (tutorialEnemy->GetIsDead())
		{
			tutorialSpriteFlag = 11;
			tutorialEnemy->Reset();
		}
	}
	//いい感じチュートリアル
	if (tutorialSpriteFlag == 11)
	{
		tutorialIikannjiTimer++;
		//時間経過でチュートリアル終了
		if (tutorialIikannjiTimer >= tutorialIikannjiMaxTime)
		{
			tutorialSpriteFlag = 12;
			tutorialIikannjiTimer = 0;
		}
	}
	//敵登場までの間
	if (tutorialSpriteFlag == 12)
	{
		tutorial12Timer++;
		tutorial1Sprite->SetPosition(tutorial1Pos + XMFLOAT2(0.0f, tutorial12Timer * 2.5f));
		//時間経過でチュートリアル終了
		if (tutorial12Timer >= tutorial12MaxTime)
		{
			tutorialSpriteFlag = 13;
			tutorial12Timer = 0;
			tutorial1Sprite->SetPosition(tutorial1Pos);
			blackSprite1->SetPosition(XMFLOAT2(0.0f, -620.0f));
			blackSprite1->SetScale(XMFLOAT2(1280.0f, 720.0f));
			blackSprite1->SetAlpha(1.0f);
			blackSprite2->SetPosition(XMFLOAT2(0.0f, 620.0f));
			blackSprite2->SetScale(XMFLOAT2(1280.0f, 720.0f));
			blackSprite2->SetAlpha(1.0f);
		}
	}
	if (tutorialSpriteFlag == 13)
	{
		tutorial13Timer++;
		if (tutorial13Timer >= tutorial13MaxTime)
		{
			tutorial13Timer = 0;
			tutorialSpriteFlag = 1;
			blackSprite1->SetPosition(XMFLOAT2(0.0f, 0.0f));
			TutorialFromGameFlag = true;
		}
	}
	//スプライト更新
	blackSprite1->Update();
	blackSprite2->Update();
	tutorial1Sprite->Update();
	tutorial2Sprite->Update();
	tutorial3Sprite->Update();
	tutorial4Sprite->Update();
	tutorial6Sprite->Update();
	tutorial7Sprite->Update();
	tutorial8Sprite->Update();
	tutorial10Sprite->Update();
	tutorial11Sprite->Update();

	billboardSprite->SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
	billboardSprite->SetScale(XMFLOAT3(2.5f, 0.3f, 1.0f));
	billboardSprite->Update();

	//パーティクル
	if (input_->TriggerKey(DIK_N))
	{
		/*sparkParticle2->Add(XMFLOAT3(0.0f, 10.0f, -5.0f));
		explosionParticle1->Add(XMFLOAT3(0.0f, 10.0f, -5.0f));
		explosionParticle2->Add(XMFLOAT3(0.0f, 10.0f, -5.0f));*/
	}
	sparkParticle2->Update();
	explosionParticle1->Update();
	explosionParticle2->Update();

	//ライト
	light->SetEye(XMFLOAT3(lightPos) + player->GetPosition());
	light->SetTarget(XMFLOAT3(lightTarget) + player->GetPosition());
	light->SetDir(XMFLOAT3(lightDir));
	light->Update();

	//ライト
	lightGroup->SetAmbientColor(XMFLOAT3(1, 1, 1));
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->Update();

	//天球
	skySphereObject->HomingUpdate(player->GetPosition());
	skySphereObject->Update();

	//プレイヤー
	player->SetEnemyPos(enemy->GetPosition());
	player->Update();

	//敵
	enemy->UpdateTutorial(tutorial13Timer);

	//平面
	plane->Update();

	//エディタで読み込んだオブジェクト更新
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Update();
	}

	//コライダー更新
	UpdateCollider();
}

void GameScene::UpdateGame()
{
	if (gameTimer == 0.0f)
	{
		player->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
	gameTimer += 1.0f;

	//カメラ更新
	camera_->UpdatePlayer(player->GetPosition(), player->GetRotation0());
	/*camera_->DebugUpdate();*/
	camera_->Update();

	//判定 デバッグ
	if (input_->PushKey(DIK_I))
	{
		camera_->SetDebugFlag(true);
	}
	else
	{
		camera_->SetDebugFlag(false);
	}

	//デバッグ用
	elecParticle->Add(XMFLOAT3(0.0f,5.0f,0.0f), 1.0f);
	elecObject->Update();
	//更新
	elecParticle->Update();
	if (input_->TriggerKey(DIK_N))
	{
		elecParticle2->Add(XMFLOAT3(10.0f, 5.0f, 0.0f), XMFLOAT3(-10.0f, 5.0f, 0.0f), 1.0f);
	}
	//更新
	elecParticle2->Update();

	//コントローラテスト
	stickTest[0] = player->GetPosition().x;
	stickTest[1] = player->GetPosition().y;
	//スプライト更新
	game1Sprite->Update();

	if (titleFromGameTimer > 1)
	{
		blackSprite1->SetAlpha(titleFromGameTimer / 120.0f);
	}
	else
	{
		blackSprite1->SetAlpha(0.0f);
	}
	title1Sprite->Update();
	title2Sprite->Update();
	blackSprite1->Update();

	billboardSprite->SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
	billboardSprite->SetScale(XMFLOAT3(2.5f, 0.3f, 1.0f));
	billboardSprite->Update();

	//パーティクル
	
	if (input_->TriggerKey(DIK_N))
	{
		/*sparkParticle2->Add(XMFLOAT3(0.0f, 10.0f, -5.0f));
		explosionParticle1->Add(XMFLOAT3(0.0f, 10.0f, -5.0f));
		explosionParticle2->Add(XMFLOAT3(0.0f, 10.0f, -5.0f));*/
	}
	sparkParticle2->Update();
	explosionParticle1->Update();
	explosionParticle2->Update();

	//ライト
	light->SetEye(XMFLOAT3(lightPos) + player->GetPosition());
	light->SetTarget(XMFLOAT3(lightTarget) + player->GetPosition());
	light->SetDir(XMFLOAT3(lightDir));
	light->Update();

	//ライト
	lightGroup->SetAmbientColor(XMFLOAT3(1, 1, 1));
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->Update();

	//天球
	skySphereObject->HomingUpdate(player->GetPosition());
	skySphereObject->Update();

	//プレイヤー
	player->SetEnemyPos(enemy->GetPosition());
	player->Update();

	//敵
	enemy->SetPlayerPos(player->GetPosition());
	enemy->Update();

	//平面
	plane->Update();

	//エディタで読み込んだオブジェクト更新
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Update();
	}

	//コライダー更新
	UpdateCollider();
}

void GameScene::UpdateClear()
{
	//タイマー更新
	if (clearFromGameTimer <= clearFromGameTime)
	{
		clearFromGameTimer++;
	}

	//カメラ更新
	camera_->UpdateClear(enemy->GetPosition(),clearFromGameTimer);
	/*camera_->DebugUpdate();*/
	camera_->Update();

	//コントローラテスト
	stickTest[0] = player->GetPosition().x;
	stickTest[1] = player->GetPosition().y;

	//スプライト更新
	game1Sprite->Update();
	blackSprite1->SetPosition(XMFLOAT2(0.0f, -620.0f));
	blackSprite1->SetScale(XMFLOAT2(1280.0f, 720.0f));
	blackSprite1->SetAlpha(1.0f);
	blackSprite2->SetPosition(XMFLOAT2(0.0f, 620.0f));
	blackSprite2->SetScale(XMFLOAT2(1280.0f, 720.0f));
	blackSprite2->SetAlpha(1.0f);
	blackSprite1->Update();
	blackSprite2->Update();
	clear1Sprite->Update();

	billboardSprite->SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
	billboardSprite->SetScale(XMFLOAT3(2.5f, 0.3f, 1.0f));
	billboardSprite->Update();

	//パーティクル
	sparkParticle->Update();
	if ((int)clearFromGameTimer % 70 == 0)
	{
		explosionTimer -= 1;
	}

	if ((int)clearFromGameTimer % (int)explosionTimer == 0 && clearFromGameTimer <= clearFromGameTime)
	{
		//乱数を生成
		float xRand, yRand, zRand, randWidth;
		XMFLOAT3 randVec(20.0f, 20.0f, 20.0f);
		randWidth = 80.0f;
		xRand = (GetRand(100.0f - randWidth, 100.0f + randWidth) - 100.0f) / 100.0f;
		yRand = (GetRand(100.0f - randWidth, 100.0f + randWidth) - 100.0f) / 100.0f;
		zRand = (GetRand(100.0f - randWidth, 100.0f + randWidth) - 100.0f) / 100.0f;
		randVec = XMFLOAT3(randVec.x * xRand, randVec.y * yRand + 20, randVec.z * zRand);

		sparkParticle2->Add(enemy->GetPosition() + randVec);
		explosionParticle1->Add(enemy->GetPosition() + randVec);
		explosionParticle2->Add(enemy->GetPosition() + randVec);
	}
	sparkParticle2->Update();
	explosionParticle1->Update();
	explosionParticle2->Update();

	//ライト
	light->SetEye(XMFLOAT3(lightPos) + player->GetPosition());
	light->SetTarget(XMFLOAT3(lightTarget) + player->GetPosition());
	light->SetDir(XMFLOAT3(lightDir));
	light->Update();

	//ライト
	lightGroup->SetAmbientColor(XMFLOAT3(1, 1, 1));
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->Update();

	//天球
	skySphereObject->HomingUpdate(player->GetPosition());
	skySphereObject->Update();

	//プレイヤー
	player->Update();

	//敵
	enemy->Update();

	//平面
	/*plane->Update();*/

	//スペースキーでファイル読み込み更新
	if (input_->TriggerKey(DIK_SPACE))
	{
		jsonLoader->LoadFile("Resources/json/demo1.json");
		int i = 0;
		for (std::unique_ptr<FbxObject3D>& object0 : object)
		{
			//プレイヤー以外のオブジェクト更新
			if (object0->GetFileName() != "player")
			{
				object0->SetPosition(jsonLoader->GetPosition(i));
				object0->SetScale(jsonLoader->GetScale(i));
				object0->SetRotation(jsonLoader->GetRotation(i));
			}
			i++;
		}
	}
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Update();
	}

	//コライダー更新
	UpdateCollider();
}

void GameScene::UpdateCollider()
{
	//事前処理
	ColliderManager::PreUpdate();

	//敵の弾と時機の当たり判定
	if (player->GetInvincibleFlag() == false)
	{
		for (int i = 0; i < enemy->GetBulletNum(); i++)
		{
			if (ColliderManager::CheckCollider(player->GetColliderData(), enemy->GetBulletColliderData(i)))
			{
				//自機にヒットフラグ送信
				player->HitEnemy();
			}
		}
	}

	//敵と時機の当たり判定
	if (player->GetInvincibleFlag() == false)
	{
		if (ColliderManager::CheckCollider(player->GetColliderData(), enemy->GetColliderData()))
		{
			//自機にヒットフラグ送信
			player->HitEnemy();
		}
	}

	//時機の弾(炎)と敵の当たり判定
	for (int i = 0; i < player->GetBullet1Num(); i++)
	{
		if (ColliderManager::CheckCollider(enemy->GetColliderData(), player->GetBullet1ColliderData(i)))
		{
			//敵にヒットフラグ送信
			enemy->HitBullet1();
			//自機にヒットフラグ送信
			player->HitBullet1(i);
		}
	}

	//時機の弾(炎)と敵の当たり判定
	for (int i = 0; i < player->GetBullet1Num(); i++)
	{
		if (ColliderManager::CheckCollider(tutorialEnemy->GetColliderData(), player->GetBullet1ColliderData(i)))
		{
			//敵にヒットフラグ送信
			tutorialEnemy->HitBullet1();
			//自機にヒットフラグ送信
			player->HitBullet1(i);
		}
	}

	//敵の弾と平面の判定
	for (std::unique_ptr<FbxObject3D>& object1 : object)
	{
		if (object1->GetFileName() == "plane")
		{
			for (int i = 0; i < enemy->GetBulletNum(); i++)
			{
				if (ColliderManager::CheckCollider(object1->GetColliderData(), enemy->GetBulletColliderData(i)))
				{
					//当たったらパーティクル発生
					sparkParticle2->Add(XMFLOAT3(enemy->GetBulletColliderData(i).center));
					explosionParticle1->Add(XMFLOAT3(enemy->GetBulletColliderData(i).center));
					explosionParticle2->Add(XMFLOAT3(enemy->GetBulletColliderData(i).center));
				}
			}
		}
	}

	//後処理
	ColliderManager::PostUpdate();
}

void GameScene::Update()
{
	//コントローラー更新
	dxInput->InputProcess();

	//モードマネージャー
	ModeManager();

	//シーンごとの処理
	(this->*Mode[mode])();
}

void GameScene::Draw()
{
	//シーンごとの描画
	(this->*ModeDraw[modeDraw])();
}

void GameScene::DrawFBXLightView()
{
	//シーンごとの描画
	(this->*ModeDrawLightView[modeDrawLightView])();
}

void GameScene::DrawTitle()
{
	//FBXの描画
	/*if (*drawFbx == 1)DrawFBXTitle();*/

	//天球描画
	skySphereObject->Draw(dxCommon_->GetCommandList(), skySphereModel->vbView, skySphereModel->ibView);

	player->Draw(dxCommon_->GetCommandList());

	//スプライトの描画
	/*if (*drawSprite == 1)*/DrawSpriteTitle();
}

void GameScene::DrawTutorial()
{
	//ImGui
	/*ImGui::Begin("GameScene");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputInt("DrawFbx", drawFbx);
	ImGui::InputInt("DrawSprite", drawSprite);
	ImGui::InputInt("DrawCollider", drawCollider);
	ImGui::InputInt("DrawParticle", drawParticle);
	ImGui::End();

	ImGui::Begin("GameScene light");
	ImGui::SetWindowPos(ImVec2(500, 0));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputFloat3("lightPos", lightPos);
	ImGui::InputFloat3("lightTarget", lightTarget);
	ImGui::InputFloat3("lightDir", lightDir);
	ImGui::End();*/

	//ImGui
	/*ImGui::Begin("GameScene");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputFloat2("StickTest", stickTest);
	ImGui::End();*/

	//天球描画
	skySphereObject->Draw(dxCommon_->GetCommandList(), skySphereModel->vbView, skySphereModel->ibView);

	//コライダーの描画
	if (*drawCollider == 1)DrawColliderTutorial();
	//FBXの描画
	if (*drawFbx == 1)DrawFBXTutorial();
	//パーティクルの描画
	if (*drawParticle == 1)DrawParticleTutorial();
	//スプライトの描画
	if (*drawSprite == 1)DrawSpriteTutorial();
}

void GameScene::DrawGame()
{
	//ImGui
	/*ImGui::Begin("GameScene");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputInt("DrawFbx", drawFbx);
	ImGui::InputInt("DrawSprite", drawSprite);
	ImGui::InputInt("DrawCollider", drawCollider);
	ImGui::InputInt("DrawParticle", drawParticle);
	ImGui::End();

	ImGui::Begin("GameScene light");
	ImGui::SetWindowPos(ImVec2(500, 0));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputFloat3("lightPos", lightPos);
	ImGui::InputFloat3("lightTarget", lightTarget);
	ImGui::InputFloat3("lightDir", lightDir);
	ImGui::End();*/

	//ImGui
	/*ImGui::Begin("GameScene");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputFloat2("StickTest", stickTest);
	ImGui::End();*/

	//天球描画
	skySphereObject->Draw(dxCommon_->GetCommandList(), skySphereModel->vbView, skySphereModel->ibView);

	//コライダーの描画
	if (*drawCollider == 1)DrawColliderGame();
	//FBXの描画
	if (*drawFbx == 1)DrawFBXGame();
	//パーティクルの描画
	if (*drawParticle == 1)DrawParticleGame();
	//スプライトの描画
	if (*drawSprite == 1)DrawSpriteGame();
}

void GameScene::DrawClear()
{
	//天球描画
	skySphereObject->Draw(dxCommon_->GetCommandList(), skySphereModel->vbView, skySphereModel->ibView);

	//コライダーの描画
	if (*drawCollider == 1)DrawColliderClear();
	//FBXの描画
	if (*drawFbx == 1)DrawFBXClear();
	//パーティクルの描画
	if (*drawParticle == 1)DrawParticleClear();
	//スプライトの描画
	if (*drawSprite == 1)DrawSpriteClear();
}

void GameScene::DrawFBXTitle()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Draw(dxCommon_->GetCommandList());
	}

	player->Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawColliderTitle()
{
	ColliderManager::Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawSpriteTitle()
{
	title1Sprite->Draw(dxCommon_->GetCommandList());
	title2Sprite->Draw(dxCommon_->GetCommandList());
	blackSprite1->Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawParticleTitle()
{
}

void GameScene::DrawFBXTutorial()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Draw(dxCommon_->GetCommandList());
	}

	player->Draw(dxCommon_->GetCommandList());
	enemy->Draw(dxCommon_->GetCommandList());
	if (tutorialSpriteFlag == 10)tutorialEnemy->Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawColliderTutorial()
{
}

void GameScene::DrawSpriteTutorial()
{
	//スプライト更新
	blackSprite1->Draw(dxCommon_->GetCommandList());
	if (tutorialSpriteFlag < 13)tutorial1Sprite->Draw(dxCommon_->GetCommandList());
	if(tutorialSpriteFlag == 2)tutorial2Sprite->Draw(dxCommon_->GetCommandList());
	if (tutorialSpriteFlag == 3 || tutorialSpriteFlag == 5 || tutorialSpriteFlag == 9)tutorial3Sprite->Draw(dxCommon_->GetCommandList());
	if (tutorialSpriteFlag == 4)tutorial4Sprite->Draw(dxCommon_->GetCommandList());
	if (tutorialSpriteFlag == 6)tutorial6Sprite->Draw(dxCommon_->GetCommandList());
	if (tutorialSpriteFlag == 7)tutorial7Sprite->Draw(dxCommon_->GetCommandList());
	if (tutorialSpriteFlag == 8)tutorial8Sprite->Draw(dxCommon_->GetCommandList());
	if (tutorialSpriteFlag == 10)tutorial10Sprite->Draw(dxCommon_->GetCommandList());
	if (tutorialSpriteFlag == 11)tutorial11Sprite->Draw(dxCommon_->GetCommandList());
	if (tutorialSpriteFlag == 13)blackSprite2->Draw(dxCommon_->GetCommandList());

	tutorialEnemy->DrawSprite(dxCommon_->GetCommandList());
}

void GameScene::DrawParticleTutorial()
{
	//プレイヤーのパーティクル描画
	player->DrawParticle(dxCommon_->GetCommandList());
}

void GameScene::DrawFBXLightViewTitle()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->DrawLightView(dxCommon_->GetCommandList());
	}

	player->DrawLightView(dxCommon_->GetCommandList());
}

void GameScene::DrawFBXLightViewTutorial()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->DrawLightView(dxCommon_->GetCommandList());
	}
	player->DrawLightView(dxCommon_->GetCommandList());
	enemy->DrawLightView(dxCommon_->GetCommandList());
	if(tutorialSpriteFlag == 10)tutorialEnemy->DrawLightView(dxCommon_->GetCommandList());
}

void GameScene::DrawFBXLightViewGame()
{
	/*for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->DrawLightView(dxCommon_->GetCommandList());
	}*/

	player->DrawLightView(dxCommon_->GetCommandList());
	enemy->DrawLightView(dxCommon_->GetCommandList());
	plane->DrawLightView(dxCommon_->GetCommandList());
}

void GameScene::DrawFBXLightViewClear()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->DrawLightView(dxCommon_->GetCommandList());
	}

	player->DrawLightView(dxCommon_->GetCommandList());
	enemy->DrawLightView(dxCommon_->GetCommandList());
}

void GameScene::ModeManager()
{
	//タイトルの時AかSPACEが押されたら
	if (input_->TriggerKey(DIK_SPACE) || dxInput->TriggerKey(DXInput::PAD_A) && mode == static_cast<size_t>(Mode::Title))
	{
		//タイトルからゲームに移るフラグオン
		gameFromTitleFlag = true;
	}
	//タイトルからゲームに移るフラグが立ったら
	if (gameFromTitleFlag == true)
	{
		//タイマー加算
		gameFromTitleTimer++;
		//タイマーが規定のタイムに達したら
		if (gameFromTitleTimer >= gameFromTitleTime)
		{
			//モードをゲームへ
			mode = static_cast<size_t>(Mode::Tutorial);
			modeDraw = static_cast<size_t>(ModeDraw::TutorialDraw);
			modeDrawLightView = static_cast<size_t>(ModeDrawLightView::TutorialDrawLightView);
			//タイマーリセット
			gameFromTitleTimer = 0.0f;
			//フラグをもとに戻す
			gameFromTitleFlag = false;
		}
	}

	//チュートリアルからゲームに移るフラグが立ったら
	if (TutorialFromGameFlag == true)
	{
		//リセット
		player->Reset();
		enemy->Reset();
		//モードをゲームへ
		mode = static_cast<size_t>(Mode::Game);
		modeDraw = static_cast<size_t>(ModeDraw::GameDraw);
		modeDrawLightView = static_cast<size_t>(ModeDrawLightView::GameDrawLightView);
		TutorialFromGameFlag = false;
	}

	//ゲームの時Bが押されたら
	//if (dxInput->TriggerKey(DXInput::PAD_B))
	//{
	//	//ゲームからゲームに移るフラグオン
	//	titleFromGameFlag = true;
	//}
	//ゲームからタイトルに移るフラグが立ったら
	if (titleFromGameFlag == true)
	{
		//タイマー加算
		titleFromGameTimer++;
		//タイマーが規定のタイムに達したら
		if (titleFromGameTimer >= titleFromGameTime)
		{
			//リセット
			player->Reset();
			enemy->Reset();
			//モードをゲームへ
			mode = static_cast<size_t>(Mode::Title);
			modeDraw = static_cast<size_t>(ModeDraw::TitleDraw);
			modeDrawLightView = static_cast<size_t>(ModeDrawLightView::TitleDrawLightView);
			//タイマーリセット
			titleFromGameTimer = 0.0f;
			//フラグをもとに戻す
			titleFromGameFlag = false;
		}
	}

	//ゲームクリア デバッグ用
	if (enemy->GetIsDead() == true)
	{
		//ゲームからゲームに移るフラグオン
		clearFromGameFlag = true;
	}
	//ゲームからクリアに移るフラグが立ったら
	if (clearFromGameFlag == true)
	{
		//モードをゲームへ
		mode = static_cast<size_t>(Mode::Clear);
		modeDraw = static_cast<size_t>(ModeDraw::ClearDraw);
		modeDrawLightView = static_cast<size_t>(ModeDrawLightView::ClearDrawLightView);
		//フラグをもとに戻す
		clearFromGameFlag = false;
	}

	//クリアの時AかSPACEが押されたら
	if (input_->TriggerKey(DIK_SPACE) || dxInput->TriggerKey(DXInput::PAD_X) && mode == static_cast<size_t>(Mode::Clear))
	{
		if (clearFromGameTimer >= clearFromGameTime)
		{
			//タイトルからゲームに移るフラグオン
			gameFromClearFlag = true;
		}
	}
	//クリアからゲームに移るフラグが立ったら
	if (gameFromClearFlag == true)
	{
		//リセット
		player->Reset();
		enemy->Reset();
		//モードをゲームへ
		mode = static_cast<size_t>(Mode::Game);
		modeDraw = static_cast<size_t>(ModeDraw::GameDraw);
		modeDrawLightView = static_cast<size_t>(ModeDrawLightView::GameDrawLightView);
		clearFromGameTimer = 0.0f;
		explosionTimer = explosionTime;
		//フラグをもとに戻す
		gameFromClearFlag = false;
	}
}

void GameScene::DrawFBXGame()
{
	/*for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Draw(dxCommon_->GetCommandList());
	}*/

	player->Draw(dxCommon_->GetCommandList());
	enemy->Draw(dxCommon_->GetCommandList());
	plane->Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawColliderGame()
{
	ColliderManager::Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawSpriteGame()
{
	game1Sprite->Draw(dxCommon_->GetCommandList());
	/*blackSprite1->Draw(dxCommon_->GetCommandList());*/

	//プレイヤーのスプライト描画
	player->DrawSprite(dxCommon_->GetCommandList());
	//敵
	enemy->DrawSprite(dxCommon_->GetCommandList());
}

void GameScene::DrawParticleGame()
{
	sparkParticle->Draw(dxCommon_->GetCommandList());
	sparkParticle2->Draw(dxCommon_->GetCommandList());
	explosionParticle1->Draw(dxCommon_->GetCommandList());
	explosionParticle2->Draw(dxCommon_->GetCommandList());

	//電気パーティクル デバッグ用
	/*elecParticle->Draw(dxCommon_->GetCommandList());*/
	elecParticle2->Draw(dxCommon_->GetCommandList());
	elecObject->Draw(dxCommon_->GetCommandList());

	//プレイヤーのパーティクル描画
	player->DrawParticle(dxCommon_->GetCommandList());

	//敵のパーティクル描画
	enemy->DrawParticle(dxCommon_->GetCommandList());
}

void GameScene::DrawFBXClear()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Draw(dxCommon_->GetCommandList());
	}

	player->Draw(dxCommon_->GetCommandList());
	enemy->Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawColliderClear()
{
	ColliderManager::Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawSpriteClear()
{
	/*game1Sprite->Draw(dxCommon_->GetCommandList());*/
	blackSprite1->Draw(dxCommon_->GetCommandList());
	blackSprite2->Draw(dxCommon_->GetCommandList());
	if(clearFromGameTimer >= clearFromGameTime)clear1Sprite->Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawParticleClear()
{
	sparkParticle->Draw(dxCommon_->GetCommandList());
	sparkParticle2->Draw(dxCommon_->GetCommandList());
	explosionParticle1->Draw(dxCommon_->GetCommandList());
	explosionParticle2->Draw(dxCommon_->GetCommandList());
}

void GameScene::SetSRV(ID3D12DescriptorHeap* SRV)
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->SetSRV(SRV);
	}

	player->SetSRV(SRV);
	enemy->SetSRV(SRV);
	plane->SetSRV(SRV);
	tutorialEnemy->SetSRV(SRV);
}

DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}

//メンバ関数のポインタテーブル
void(GameScene::* GameScene::Mode[])() =
{
	&GameScene::UpdateTitle,
	&GameScene::UpdateTutorial,
	&GameScene::UpdateGame,
	&GameScene::UpdateClear,
};
void(GameScene::* GameScene::ModeDraw[])() =
{
	&GameScene::DrawTitle,
	&GameScene::DrawTutorial,
	&GameScene::DrawGame,
	&GameScene::DrawClear,
};
void(GameScene::* GameScene::ModeDrawLightView[])() =
{
	&GameScene::DrawFBXLightViewTitle,
	&GameScene::DrawFBXLightViewTutorial,
	&GameScene::DrawFBXLightViewGame,
	&GameScene::DrawFBXLightViewClear,
};