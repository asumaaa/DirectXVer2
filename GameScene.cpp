#include "GameScene.h"
#include "FbxLoader.h"
#include<fstream>
#include "sstream"
#include "stdio.h"
#include "string.h"
#include "vector"
#include "imgui.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	this->dxCommon_ = dxCommon;
	this->input_ = input;

	//FBXローダー初期化
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("plane", "Resources/pictures/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree", "Resources/pictures/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree1", "Resources/pictures/black.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree2", "Resources/pictures/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree3", "Resources/pictures/black.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Cube", "Resources/pictures/toriko.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("player", "Resources/pictures/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("enemy", "Resources/pictures/toriko.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("playerBullet", "Resources/pictures/white1x1.png"));

	//スプライトマネージャー
	SpriteManager::SetDevice(dxCommon->GetDevice());
	SpriteManager* newSpriteManager = new SpriteManager();
	newSpriteManager->Initialize();
	newSpriteManager->LoadFile(0, L"Resources/pictures/toriko.png");
	newSpriteManager->LoadFile(1, L"Resources/pictures/toriko2.png");
	newSpriteManager->LoadFile(2, L"Resources/pictures/GourmetSpyzer.png");
	newSpriteManager->LoadFile(3, L"Resources/pictures/orange.png");
	newSpriteManager->LoadFile(4, L"Resources/pictures/red.png");
	newSpriteManager->LoadFile(5, L"Resources/pictures/effect1.png");
	newSpriteManager->LoadFile(6, L"Resources/pictures/effect2.png");
	newSpriteManager->LoadFile(7, L"Resources/pictures/effect3.png");
	spriteManager.reset(newSpriteManager);

	//スプライト
	Sprite::SetDevice(dxCommon->GetDevice());
	Sprite::SetSpriteManager(spriteManager.get());
	Sprite::CreateGraphicsPipeLine();

	//カメラ初期化
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetEye({ 0.0f,10.0f,5.0f });
	camera_->SetTarget({ 0.0f,0.0f,0.0f });

	//ライト(影)
	Light* newLight = new Light();
	newLight->Initialize();
	light.reset(newLight);

	//ライト
	LightGroup::StaticInitialize(dxCommon_->GetDevice());
	LightGroup* newLightGroup = new LightGroup();
	newLightGroup = LightGroup::Create();
	lightGroup.reset(newLightGroup);

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

	//パーティクル
	BaseParticle::SetSpriteManager(spriteManager.get());
	BaseParticle::SetDevice(dxCommon_->GetDevice());
	BaseParticle::SetCamera(camera_.get());
	BaseParticle::SetInput(input_);
	BaseParticle::CreateGraphicsPipeline();
	BaseParticle* newParticleManager = new BaseParticle();
	newParticleManager->CreateBuffers();
	newParticleManager->SetTextureNum(5);
	particleManager.reset(newParticleManager);

	//弾けるパーティクル
	SparkParticle::SetSpriteManager(spriteManager.get());
	SparkParticle::SetDevice(dxCommon_->GetDevice());
	SparkParticle::SetCamera(camera_.get());
	SparkParticle::SetInput(input_);
	SparkParticle::CreateGraphicsPipeline();
	SparkParticle* newSparkParticle = new SparkParticle();
	newSparkParticle->CreateBuffers();
	newSparkParticle->SetTextureNum(5);
	sparkParticle.reset(newSparkParticle);

	//プレイヤーの弾
	PlayerBullet::SetCamera(camera_.get());
	PlayerBullet::SetInput(input);
	PlayerBullet* newPlayerBullet = new PlayerBullet();
	//モデルセット
	for (std::unique_ptr<FbxModel>& model : models)
	{
		if (model->GetFileName() == "playerBullet")
		{
			newPlayerBullet->SetModel(model.get());
		}
	}
	newPlayerBullet->Initialize();
	playerBullet.reset(newPlayerBullet);

	//プレイヤー
	Player::SetCamera(camera_.get());
	Player::SetInput(input);
	/*Player::SetDXInput()*/
	Player* newPlayer = new Player();
	newPlayer->Initialize();
	newPlayer->SetBullet(playerBullet.get());
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

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLight(light.get());
	FbxObject3D::SetLightGroup(lightGroup.get());
	FbxObject3D::CreateGraphicsPipelineLightView();
	FbxObject3D::CreateGraphicsPipeline();

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

		object.push_back(std::move(newObject));

		//プレイヤーのオブジェクトがあったら
		if (jsonLoader->GetFileName(i) == "player")
		{
			player->SetObject(object.back().get());
			/*object.pop_back();*/
		}
		//敵のオブジェクトがあったら
		if (jsonLoader->GetFileName(i) == "enemy")
		{
			enemy->SetObject(object.back().get());
		}
		//平面のオブジェクトがあったら
	/*	if (jsonLoader->GetFileName(i) == "plane")
		{
			plane->SetObject(object.back().get());
		}*/

		//コライダーのセット
		ColliderManager::SetCollider(jsonLoader->GetColliderData(i));
	}
}

void GameScene::Finalize()
{
	models.clear();
	delete spriteManager.get();
	delete lightGroup.get();
	object.clear();
	/*delete tree1csv;
	delete spriteManager;
	delete lightGroup;*/
}

void GameScene::Update()
{
	//カメラ更新
	camera_->UpdatePlayer(player->GetPosition(),player->GetRotation());
	camera_->Update();
	//コントローラー更新
	dxInput->InputProcess();

	/*particleObject->SetPosition(XMFLOAT3(10.0f,5.0f,0));*/
	//パーティクル
	/*particleManager->Update();*/
	if (input_->TriggerKey(DIK_N))
	{
		sparkParticle->Add(XMFLOAT3(0,0,0));
	}
	sparkParticle->Update();

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

	//プレイヤー
	player->Update();

	//敵
	enemy->Update();

	//平面
	/*plane->Update();*/

	//スペースキーでファイル読み込み更新
	if(input_->TriggerKey(DIK_SPACE))
	{
		jsonLoader->LoadFile("Resources/json/demo.json");
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
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		if (object0->GetFileName() == "player")
		{
			for (std::unique_ptr<FbxObject3D>& object1 : object)
			{
				if (object1->GetFileName() == "plane")
				{
					//当たっていたら
					while (ColliderManager::CheckCollider(object0->GetColliderData(), object1->GetColliderData()))
					{
						player->HitPlane();
					}
				}
			}
		}
	}

	//プレイヤーと平面との判定
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		if (object0->GetFileName() == "player")
		{
			for (std::unique_ptr<FbxObject3D>& object1 : object)
			{
				if (object1->GetFileName() == "enemy")
				{
					ColliderManager::CheckCollider(object0->GetColliderData(), object1->GetColliderData());
				}
			}
		}
	}

	//弾と敵との判定
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		if (object0->GetFileName() == "enemy")
		{
			//弾が一つ以上あれば
			if (playerBullet->GetBulletNum() >= 1)
			{
				for (int i = 0; i < playerBullet->GetBulletNum(); i++)
				{
					if (ColliderManager::CheckCollider(playerBullet->GetColliderData(i),
						object0->GetColliderData()))
					{
						//パーティクル
						sparkParticle->Add(playerBullet->GetPosition(i));
						//弾
						playerBullet->SetHitFlag(true, i);
					}
				}
			}
		}
	}

	//後処理
	ColliderManager::PostUpdate();
}

void GameScene::Draw()
{
	//ImGui
	ImGui::Begin("GameScene");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputInt("DrawFbx", drawFbx);
	ImGui::InputInt("DrawSprite", drawSprite);
	ImGui::InputInt("DrawCollider", drawCollider);
	ImGui::InputInt("DrawParticle", drawParticle);
	ImGui::End();

	//コライダーの描画
	if (*drawCollider == 1)DrawCollider();
	//スプライトの描画
	if (*drawSprite == 1)DrawSprite();
	//FBXの描画
	if (*drawFbx == 1)DrawFBX();
	//パーティクルの描画
	if (*drawParticle == 1)DrawParticle();
}

void GameScene::DrawFBXLightView()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->DrawLightView(dxCommon_->GetCommandList());
	}

	//プレイヤー
	player->DrawLightView(dxCommon_->GetCommandList());
}

void GameScene::DrawFBX()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Draw(dxCommon_->GetCommandList());
	}
	//プレイヤー
	player->Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawCollider()
{
	ColliderManager::Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawSprite()
{
	/*enemy->DrawSprite(dxCommon_->GetCommandList());*/
}

void GameScene::DrawParticle()
{
	sparkParticle->Draw(dxCommon_->GetCommandList());
}

void GameScene::SetSRV(ID3D12DescriptorHeap* SRV)
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->SetSRV(SRV);
	}
	//プレイヤー
	player->SetSRV(SRV);
}

DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}
