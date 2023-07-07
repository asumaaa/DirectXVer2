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

	//FBX���[�_�[������
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//���f�������w�肵�ăt�@�C���ǂݍ���
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("plane", "Resources/pictures/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree", "Resources/pictures/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree1", "Resources/pictures/black.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree2", "Resources/pictures/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree3", "Resources/pictures/black.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Cube", "Resources/pictures/toriko.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("player", "Resources/pictures/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("enemy", "Resources/pictures/toriko.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("playerBullet", "Resources/pictures/white1x1.png"));

	//�X�v���C�g�}�l�[�W���[
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

	//�X�v���C�g
	Sprite::SetDevice(dxCommon->GetDevice());
	Sprite::SetSpriteManager(spriteManager.get());
	Sprite::CreateGraphicsPipeLine();

	//�J����������
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetEye({ 0.0f,10.0f,5.0f });
	camera_->SetTarget({ 0.0f,0.0f,0.0f });

	//���C�g(�e)
	Light* newLight = new Light();
	newLight->Initialize();
	light.reset(newLight);

	//���C�g
	LightGroup::StaticInitialize(dxCommon_->GetDevice());
	LightGroup* newLightGroup = new LightGroup();
	newLightGroup = LightGroup::Create();
	lightGroup.reset(newLightGroup);

	//�R���C�_�[�̃L���[�u
	ColliderCubeModel* newCubeModel = new ColliderCubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice());
	colliderCubeModel.reset(newCubeModel);
	ColliderCubeObject::SetDevice(dxCommon_->GetDevice());
	ColliderCubeObject::SetCamera(camera_.get());
	ColliderCubeObject::SetInput(input_);
	ColliderCubeObject::SetModel(colliderCubeModel.get());
	ColliderCubeObject::CreateGraphicsPipeline();

	//�R���C�_�[�̋�
	ColliderSphereModel* newSphereModel = new ColliderSphereModel();
	newSphereModel->CreateBuffers(dxCommon_->GetDevice());
	colliderSphereModel.reset(newSphereModel);
	ColliderSphereObject::SetDevice(dxCommon_->GetDevice());
	ColliderSphereObject::SetCamera(camera_.get());
	ColliderSphereObject::SetInput(input_);
	ColliderSphereObject::SetModel(colliderSphereModel.get());
	ColliderSphereObject::CreateGraphicsPipeline();

	//�R���C�_�[�̕���
	ColliderPlaneModel* newPlaneModel = new ColliderPlaneModel();
	newPlaneModel->CreateBuffers(dxCommon_->GetDevice());
	colliderPlaneModel.reset(newPlaneModel);
	ColliderPlaneObject::SetDevice(dxCommon_->GetDevice());
	ColliderPlaneObject::SetCamera(camera_.get());
	ColliderPlaneObject::SetInput(input_);
	ColliderPlaneObject::SetModel(colliderPlaneModel.get());
	ColliderPlaneObject::CreateGraphicsPipeline();

	//�R���C�_�[�}�l�[�W���[
	ColliderManager::SetColliderCubeModel(colliderCubeModel.get());
	ColliderManager::SetColliderSphereModel(colliderSphereModel.get());
	ColliderManager* newColliderManager = new ColliderManager();
	colliderManager.reset(newColliderManager);

	//�p�[�e�B�N��
	BaseParticle::SetSpriteManager(spriteManager.get());
	BaseParticle::SetDevice(dxCommon_->GetDevice());
	BaseParticle::SetCamera(camera_.get());
	BaseParticle::SetInput(input_);
	BaseParticle::CreateGraphicsPipeline();
	BaseParticle* newParticleManager = new BaseParticle();
	newParticleManager->CreateBuffers();
	newParticleManager->SetTextureNum(5);
	particleManager.reset(newParticleManager);

	//�e����p�[�e�B�N��
	SparkParticle::SetSpriteManager(spriteManager.get());
	SparkParticle::SetDevice(dxCommon_->GetDevice());
	SparkParticle::SetCamera(camera_.get());
	SparkParticle::SetInput(input_);
	SparkParticle::CreateGraphicsPipeline();
	SparkParticle* newSparkParticle = new SparkParticle();
	newSparkParticle->CreateBuffers();
	newSparkParticle->SetTextureNum(5);
	sparkParticle.reset(newSparkParticle);

	//�v���C���[�̒e
	PlayerBullet::SetCamera(camera_.get());
	PlayerBullet::SetInput(input);
	PlayerBullet* newPlayerBullet = new PlayerBullet();
	//���f���Z�b�g
	for (std::unique_ptr<FbxModel>& model : models)
	{
		if (model->GetFileName() == "playerBullet")
		{
			newPlayerBullet->SetModel(model.get());
		}
	}
	newPlayerBullet->Initialize();
	playerBullet.reset(newPlayerBullet);

	//�v���C���[
	Player::SetCamera(camera_.get());
	Player::SetInput(input);
	/*Player::SetDXInput()*/
	Player* newPlayer = new Player();
	newPlayer->Initialize();
	newPlayer->SetBullet(playerBullet.get());
	player.reset(newPlayer);

	//�G
	Enemy::SetCamera(camera_.get());
	Enemy::SetInput(input);
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize();
	enemy.reset(newEnemy);

	//����
	/*Plane::SetCamera(camera_.get());
	Plane::SetInput(input);*/
	/*Plane* newPlane = new Plane();
	newPlane->Initialize();
	plane.reset(newPlane);*/

	//�f�o�C�X���Z�b�g
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLight(light.get());
	FbxObject3D::SetLightGroup(lightGroup.get());
	FbxObject3D::CreateGraphicsPipelineLightView();
	FbxObject3D::CreateGraphicsPipeline();

	//���x���G�f�B�^
	JSONLoader* newJsonLoader = new JSONLoader();
	newJsonLoader->LoadFile("Resources/json/demo.json");
	jsonLoader.reset(newJsonLoader);

	for (int i = 0; i < jsonLoader->GetObjectNum(); i++)
	{
		std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
		//�I�u�W�F�N�g������
		newObject->Initialize();

		//���f���Z�b�g
		for (std::unique_ptr<FbxModel>& model : models)
		{
			if (jsonLoader->GetFileName(i) == model->GetFileName())
			{
				newObject->SetModel(model.get());
			}
		}

		//�I�u�W�F�N�g�̔z�u
		newObject->SetObjectData(jsonLoader->GetObjectData(i));
		//�R���C�_�[�̔z�u
		newObject->SetColliderData(jsonLoader->GetColliderData(i));

		object.push_back(std::move(newObject));

		//�v���C���[�̃I�u�W�F�N�g����������
		if (jsonLoader->GetFileName(i) == "player")
		{
			player->SetObject(object.back().get());
			/*object.pop_back();*/
		}
		//�G�̃I�u�W�F�N�g����������
		if (jsonLoader->GetFileName(i) == "enemy")
		{
			enemy->SetObject(object.back().get());
		}
		//���ʂ̃I�u�W�F�N�g����������
	/*	if (jsonLoader->GetFileName(i) == "plane")
		{
			plane->SetObject(object.back().get());
		}*/

		//�R���C�_�[�̃Z�b�g
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
	//�J�����X�V
	camera_->UpdatePlayer(player->GetPosition(),player->GetRotation());
	camera_->Update();
	//�R���g���[���[�X�V
	dxInput->InputProcess();

	/*particleObject->SetPosition(XMFLOAT3(10.0f,5.0f,0));*/
	//�p�[�e�B�N��
	/*particleManager->Update();*/
	if (input_->TriggerKey(DIK_N))
	{
		sparkParticle->Add(XMFLOAT3(0,0,0));
	}
	sparkParticle->Update();

	//���C�g
	light->SetEye(XMFLOAT3(lightPos));
	light->SetTarget(XMFLOAT3(lightTarget));
	light->SetDir(XMFLOAT3(lightDir));
	light->Update();

	//���C�g
	lightGroup->SetAmbientColor(XMFLOAT3(1, 1, 1));
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->Update();

	//�v���C���[
	player->Update();

	//�G
	enemy->Update();

	//����
	/*plane->Update();*/

	//�X�y�[�X�L�[�Ńt�@�C���ǂݍ��ݍX�V
	if(input_->TriggerKey(DIK_SPACE))
	{
		jsonLoader->LoadFile("Resources/json/demo.json");
		int i = 0;
		for (std::unique_ptr<FbxObject3D>& object0 : object)
		{
			//�v���C���[�ȊO�̃I�u�W�F�N�g�X�V
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

	//�R���C�_�[�X�V
	UpdateCollider();
}

void GameScene::UpdateCollider()
{
	//���O����
	ColliderManager::PreUpdate();

	//�v���C���[�ƕ��ʂƂ̔���
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		if (object0->GetFileName() == "player")
		{
			for (std::unique_ptr<FbxObject3D>& object1 : object)
			{
				if (object1->GetFileName() == "plane")
				{
					//�������Ă�����
					while (ColliderManager::CheckCollider(object0->GetColliderData(), object1->GetColliderData()))
					{
						player->HitPlane();
					}
				}
			}
		}
	}

	//�v���C���[�ƕ��ʂƂ̔���
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

	//�e�ƓG�Ƃ̔���
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		if (object0->GetFileName() == "enemy")
		{
			//�e����ȏ゠���
			if (playerBullet->GetBulletNum() >= 1)
			{
				for (int i = 0; i < playerBullet->GetBulletNum(); i++)
				{
					if (ColliderManager::CheckCollider(playerBullet->GetColliderData(i),
						object0->GetColliderData()))
					{
						//�p�[�e�B�N��
						sparkParticle->Add(playerBullet->GetPosition(i));
						//�e
						playerBullet->SetHitFlag(true, i);
					}
				}
			}
		}
	}

	//�㏈��
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

	//�R���C�_�[�̕`��
	if (*drawCollider == 1)DrawCollider();
	//�X�v���C�g�̕`��
	if (*drawSprite == 1)DrawSprite();
	//FBX�̕`��
	if (*drawFbx == 1)DrawFBX();
	//�p�[�e�B�N���̕`��
	if (*drawParticle == 1)DrawParticle();
}

void GameScene::DrawFBXLightView()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->DrawLightView(dxCommon_->GetCommandList());
	}

	//�v���C���[
	player->DrawLightView(dxCommon_->GetCommandList());
}

void GameScene::DrawFBX()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Draw(dxCommon_->GetCommandList());
	}
	//�v���C���[
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
	//�v���C���[
	player->SetSRV(SRV);
}

DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}
