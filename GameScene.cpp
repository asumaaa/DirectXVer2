#include "GameScene.h"
#include "FbxLoader.h"
#include<fstream>
#include "sstream"
#include "stdio.h"
#include "string.h"
#include "vector"
#include "imgui.h"

#define PI 3.1415

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
	camera_->SetEye({ 0, 10,-20 });

	//ライト
	light = new Light;
	light->Initialize();

	//FBX読み込み
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	modelTree = FbxLoader::GetInstance()->LoadModelFromFile("Tree", "Resources/white1x1.png");
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/grassFiled.png");
	model2 = FbxLoader::GetInstance()->LoadModelFromFile("Walking", "Resources/white1x1.png");

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLight(light);
	FbxObject3D::CreateGraphicsPipeline0();
	FbxObject3D::CreateGraphicsPipeline1();
	FbxObject3D::CreateGraphicsPipeline2();

	//オブジェクト初期化
	for (int i = 0; i < verticalTreeNum; i++)
	{
		for (int j = 0; j < horizonTreeNum; j++)
		{
			std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
			newObject->Initialize();
			newObject->SetModel(modelTree);

			newObject->SetPosition({ j * horizonTreeWidth - (horizonTreeWidth * horizonTreeNum) / 2, 0.0f,
				i * verticalTreeWidth - (verticalTreeWidth * verticalTreeNum) / 2 });
			newObject->SetScale(treeScale);
			newObject->SetRotation(treeRotation);

			objectTree.push_back(std::move(newObject));
		}
	}

	object1 = new FbxObject3D;
	object1->Initialize();
	object1->SetModel(model1);

	//スプライトマネージャー
	SpriteManager::SetDevice(dxCommon->GetDevice());
	spriteManager = new SpriteManager;
	spriteManager->Initialize();
	spriteManager->LoadFile(0,L"Resources/toriko.png");
	spriteManager->LoadFile(1, L"Resources/toriko2.png");
	spriteManager->LoadFile(2, L"Resources/GourmetSpyzer.png");
	spriteManager->LoadFile(3, L"Resources/orange.png");
	spriteManager->LoadFile(4,L"Resources/red.png");
}

void GameScene::Update()
{
	//カメラ更新
	camera_->SetEye({0.0f,10.0f,5.0f});
	camera_->DebugUpdate();
	camera_->Update();
	//コントローラー更新
	dxInput->InputProcess();

	//ライト
	light->SetEye({ -25.0f,25.0f,25.0f });
	light->SetTarget({0.0f,0.0f,0.0f});
	light->Update();

	//オブジェクト更新
	for (std::unique_ptr<FbxObject3D>& object : objectTree)
	{
		object->Update();
	}

	//オブジェクト更新
	object1->SetPosition({ -3,0,4 });
	object1->SetScale({ 1.0f,0.001f,1.0f });
	object1->SetRotation({0,0,0});
	object1->Update();
}

void GameScene::Draw()
{
	/*ImGui::Begin("Light");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 500));
	ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir0",lightDir0);
	ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("circleShadowDir", circleShadowDir);
	ImGui::InputFloat3("circleShadowAtten", circleShadowAtten);
	ImGui::ColorEdit3("pointLightColor", pointLightColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("pointLightPos", pointLightPos0);
	ImGui::InputFloat3("pointLightAtten", pointLightAtten0);
	ImGui::InputFloat3("lightPos", shadowLightPos);
	ImGui::End();*/

	/*object0->Draw0(dxCommon_->GetCommandList());*/
	/*object0->Draw1(dxCommon_->GetCommandList());*/
	/*object1->Draw0(dxCommon_->GetCommandList());*/
	/*object1->Draw1(dxCommon_->GetCommandList());*/
	/*object2->Draw0(dxCommon_->GetCommandList());*/
	/*object2->Draw1(dxCommon_->GetCommandList());*/

	Draw2();
}

void GameScene::Draw0()
{
	for (std::unique_ptr<FbxObject3D>& object : objectTree)
	{
		object->Draw0(dxCommon_->GetCommandList());
	}
	object1->Draw0(dxCommon_->GetCommandList());
}

void GameScene::Draw1()
{
	for (std::unique_ptr<FbxObject3D>& object : objectTree)
	{
		object->Draw1(dxCommon_->GetCommandList());
	}
	object1->Draw1(dxCommon_->GetCommandList());
}

void GameScene::Draw2()
{
	for (std::unique_ptr<FbxObject3D>& object : objectTree)
	{
		object->Draw2(dxCommon_->GetCommandList());
	}
	object1->Draw2(dxCommon_->GetCommandList());
}

void GameScene::SetSRV(ID3D12DescriptorHeap* SRV)
{
	for (std::unique_ptr<FbxObject3D>& object : objectTree)
	{
		object->SetSRV(SRV);
	}
	object1->SetSRV(SRV);
}

DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}
