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

	//カメラ初期化
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 0,-20 });

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
	ColliderCubeObject::CreateGraphicsPipeline();

	//FBXローダー初期化
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Stone", "Resources/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree", "Resources/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree1", "Resources/black.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree2", "Resources/white1x1.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Tree3", "Resources/black.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("Cube", "Resources/toriko.png"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("player", "Resources/white1x1.png"));

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLight(light.get());
	FbxObject3D::SetLightGroup(lightGroup.get());
	FbxObject3D::SetColliderCubeModel(colliderCubeModel.get());
	FbxObject3D::CreateGraphicsPipelineLightView();
	FbxObject3D::CreateGraphicsPipeline();

	//レベルエディタ
	JSONLoader* newJsonLoader = new JSONLoader();
	newJsonLoader->LoadFile("Resources/untitled.json");
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

		//配置
		newObject->SetPosition(jsonLoader->GetPosition(i));
		newObject->SetScale(jsonLoader->GetScale(i));
		newObject->SetRotation(jsonLoader->GetRotation(i));
		newObject->SetCollider(jsonLoader->GetColliderData(i));

		object.push_back(std::move(newObject));
	}

	//スプライトマネージャー
	SpriteManager::SetDevice(dxCommon->GetDevice());
	SpriteManager* newSpriteManager = new SpriteManager();
	newSpriteManager->Initialize();
	newSpriteManager->LoadFile(0,L"Resources/toriko.png");
	newSpriteManager->LoadFile(1, L"Resources/toriko2.png");
	newSpriteManager->LoadFile(2, L"Resources/GourmetSpyzer.png");
	newSpriteManager->LoadFile(3, L"Resources/orange.png");
	newSpriteManager->LoadFile(4,L"Resources/red.png");
	spriteManager.reset(newSpriteManager);
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
	camera_->SetEye({ 0.0f,10.0f,5.0f });
	camera_->SetTarget({ 0.0f,0.0f,0.0f });
	camera_->DebugUpdate();
	camera_->Update();
	//コントローラー更新
	dxInput->InputProcess();

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

	//スペースキーでファイル読み込み更新
	if(input_->TriggerKey(DIK_SPACE))
	{
		jsonLoader->LoadFile("Resources/untitled.json");
		int i = 0;
		for (std::unique_ptr<FbxObject3D>& object0 : object)
		{
			object0->SetPosition(jsonLoader->GetPosition(i));
			object0->SetScale(jsonLoader->GetScale(i));
			object0->SetRotation(jsonLoader->GetRotation(i));
			i++;
		}
	}
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Update();
	}
}

void GameScene::Draw()
{
	//ImGui::Begin("Light");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 150));
	///*ImGui::InputFloat3("lightDir", lightDir);*/
	//ImGui::InputFloat3("lightTarget", lightTarget);
	//ImGui::InputFloat3("lightPos", lightPos);
	///*ImGui::InputFloat3("lightAtten", lightAtten);
	//ImGui::InputFloat2("lightFactorAngle", lightFactorAngle);*/
	//ImGui::End();

	DrawFBX();
}

void GameScene::DrawFBXLightView()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->DrawLightView(dxCommon_->GetCommandList());
	}
}

void GameScene::DrawFBX()
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Draw(dxCommon_->GetCommandList());
		object0->DrawCollider(dxCommon_->GetCommandList());
	}
}

void GameScene::SetSRV(ID3D12DescriptorHeap* SRV)
{
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->SetSRV(SRV);
	}
}

DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}
