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
	newTextureManager->LoadFile(4, L"Resources/pictures/red.png");
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

	//爆発パーティクル
	ThunderParticle::SetSpriteManager(textureManager.get());
	ThunderParticle::SetDevice(dxCommon_->GetDevice());
	ThunderParticle::SetCamera(camera_.get());
	ThunderParticle::SetInput(input_);
	ThunderParticle::CreateGraphicsPipeline();

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

	//平面
	/*Plane::SetCamera(camera_.get());
	Plane::SetInput(input);*/
	/*Plane* newPlane = new Plane();
	newPlane->Initialize();
	plane.reset(newPlane);*/

	//レベルエディタ
	JSONLoader* newJsonLoader = new JSONLoader();
	newJsonLoader->LoadFile("Resources/json/demo.json");
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

void GameScene::Update()
{
	//コントローラー更新
	dxInput->InputProcess();

	//モードマネージャー
	ModeManager();

	//シーンごとの処理
	(this->*Mode[mode])();
}

void GameScene::UpdateTitle()
{
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
	/*UpdateCollider();*/
}

void GameScene::UpdateGame()
{
	//カメラ更新
	camera_->UpdatePlayer(player->GetPosition(), player->GetRotation0());
	/*camera_->DebugUpdate();*/
	camera_->Update();

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
	sparkParticle2->Update();
	explosionParticle1->Update();
	explosionParticle2->Update();

	//ライト
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
	/*sparkParticle->Update();*/
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

	//プレイヤーと平面との判定
	//for (std::unique_ptr<FbxObject3D>& object0 : object)
	//{
	//	if (object0->GetFileName() == "player")
	//	{
	//		for (std::unique_ptr<FbxObject3D>& object1 : object)
	//		{
	//			if (object1->GetFileName() == "plane")
	//			{
	//				//当たっていたら
	//				while (ColliderManager::CheckCollider(object0->GetColliderData(), object1->GetColliderData()))
	//				{
	//					player->HitPlane();
	//				}
	//			}
	//		}
	//	}
	//}

	////プレイヤーと平面との判定
	//for (std::unique_ptr<FbxObject3D>& object0 : object)
	//{
	//	if (object0->GetFileName() == "player")
	//	{
	//		for (std::unique_ptr<FbxObject3D>& object1 : object)
	//		{
	//			if (object1->GetFileName() == "enemy")
	//			{
	//				ColliderManager::CheckCollider(object0->GetColliderData(), object1->GetColliderData());
	//			}
	//		}
	//	}
	//}

	////弾と敵との判定
	//for (std::unique_ptr<FbxObject3D>& object0 : object)
	//{
	//	if (object0->GetFileName() == "enemy")
	//	{
	//		//弾が一つ以上あれば
	//		if (playerBullet->GetBulletNum() >= 1)
	//		{
	//			for (int i = 0; i < playerBullet->GetBulletNum(); i++)
	//			{
	//				if (ColliderManager::CheckCollider(playerBullet->GetColliderData(i),
	//					object0->GetColliderData()))
	//				{
	//					//パーティクル
	//					sparkParticle2->Add(XMFLOAT3(playerBullet->GetPosition(i)));
	//					explosionParticle1->Add(XMFLOAT3(playerBullet->GetPosition(i)));
	//					explosionParticle2->Add(XMFLOAT3(playerBullet->GetPosition(i)));
	//					//弾
	//					playerBullet->SetHitFlag(true, i);
	//				}
	//			}
	//		}
	//	}
	//}

	//後処理
	ColliderManager::PostUpdate();
}

void GameScene::Draw()
{
	//シーンごとの描画
	(this->*ModeDraw[modeDraw])();
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

void GameScene::DrawGame()
{
	////ImGui
	//ImGui::Begin("GameScene");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 150));
	//ImGui::InputInt("DrawFbx", drawFbx);
	//ImGui::InputInt("DrawSprite", drawSprite);
	//ImGui::InputInt("DrawCollider", drawCollider);
	//ImGui::InputInt("DrawParticle", drawParticle);
	//ImGui::End();

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
	//スプライトの描画
	if (*drawSprite == 1)DrawSpriteGame();
	//FBXの描画
	if (*drawFbx == 1)DrawFBXGame();
	//パーティクルの描画
	if (*drawParticle == 1)DrawParticleGame();
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

void GameScene::DrawFBXLightView()
{
	//シーンごとの描画
	(this->*ModeDrawLightView[modeDrawLightView])();
}

void GameScene::DrawFBXLightViewTitle()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->DrawLightView(dxCommon_->GetCommandList());
	}

	player->DrawLightView(dxCommon_->GetCommandList());
}

void GameScene::DrawFBXLightViewGame()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->DrawLightView(dxCommon_->GetCommandList());
	}

	player->DrawLightView(dxCommon_->GetCommandList());
	enemy->DrawLightView(dxCommon_->GetCommandList());
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
			mode = static_cast<size_t>(Mode::Game);
			modeDraw = static_cast<size_t>(ModeDraw::GameDraw);
			modeDrawLightView = static_cast<size_t>(ModeDrawLightView::GameDrawLightView);
			//タイマーリセット
			gameFromTitleTimer = 0.0f;
			//フラグをもとに戻す
			gameFromTitleFlag = false;
		}
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
	if (dxInput->TriggerKey(DXInput::PAD_A))
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
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Draw(dxCommon_->GetCommandList());
	}

	player->Draw(dxCommon_->GetCommandList());
	enemy->Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawColliderGame()
{
	ColliderManager::Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawSpriteGame()
{
	game1Sprite->Draw(dxCommon_->GetCommandList());
	blackSprite1->Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawParticleGame()
{
	sparkParticle->Draw(dxCommon_->GetCommandList());
	sparkParticle2->Draw(dxCommon_->GetCommandList());
	explosionParticle1->Draw(dxCommon_->GetCommandList());
	explosionParticle2->Draw(dxCommon_->GetCommandList());

	//プレイヤーのパーティクル描画
	player->DrawParticle(dxCommon_->GetCommandList());
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
}

DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}

//メンバ関数のポインタテーブル
void(GameScene::* GameScene::Mode[])() =
{
	&GameScene::UpdateTitle,
	&GameScene::UpdateGame,
	&GameScene::UpdateClear,
};
void(GameScene::* GameScene::ModeDraw[])() =
{
	&GameScene::DrawTitle,
	&GameScene::DrawGame,
	&GameScene::DrawClear,
};
void(GameScene::* GameScene::ModeDrawLightView[])() =
{
	&GameScene::DrawFBXLightViewTitle,
	&GameScene::DrawFBXLightViewGame,
	&GameScene::DrawFBXLightViewClear,
};