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
	camera_->SetEye({ 0, 5,-10 });

	//ライト
	light = new Light;
	light->Initialize();

	//FBX読み込み
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	model0 = FbxLoader::GetInstance()->LoadModelFromFile("key", "Resources/key.png");
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/toriko.png");
	model2 = FbxLoader::GetInstance()->LoadModelFromFile("Walking", "Resources/white1x1.png");

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLight(light);
	FbxObject3D::CreateGraphicsPipeline0();
	FbxObject3D::CreateGraphicsPipeline1();
	FbxObject3D::CreateGraphicsPipeline2();

	//オブジェクト初期化
	object0 = new FbxObject3D;
	object0->Initialize();
	object0->SetModel(model0);

	object1 = new FbxObject3D;
	object1->Initialize();
	object1->SetModel(model1);

	object2 = new FbxObject3D;
	object2->Initialize();
	object2->SetModel(model2);
	object2->PlayAnimation();

	//スプライトマネージャー
	SpriteManager::SetDevice(dxCommon->GetDevice());
	spriteManager = new SpriteManager;
	spriteManager->Initialize();
	spriteManager->LoadFile(0,L"Resources/toriko.png");
	spriteManager->LoadFile(1, L"Resources/toriko2.png");
	spriteManager->LoadFile(2, L"Resources/GourmetSpyzer.png");
	spriteManager->LoadFile(3, L"Resources/orange.png");
	spriteManager->LoadFile(4,L"Resources/red.png");

	//スプライト
	Sprite::SetDevice(dxCommon->GetDevice());
	Sprite::SetSpriteManager(spriteManager);
	Sprite::CreateGraphicsPipeLine();

	sprite0 = new Sprite;
	sprite0->SetTextureNum(0);
	sprite0->Initialize();

	sprite1 = new Sprite;
	sprite1->SetTextureNum(1);
	sprite1->Initialize();

	sprite2 = new Sprite;
	sprite2->SetTextureNum(2);
	sprite2->Initialize();
}

void GameScene::Update()
{
	//カメラ更新
	camera_->DebugUpdate();
	camera_->Update();
	//コントローラー更新
	dxInput->InputProcess();

	//ライト
	light->SetEye({-10.0f,10.0f,10.0f});
	light->Update();

	//オブジェクト更新
	rotation0.y += 0.02;
	object0->SetPosition({ -5,3,0 });
	object0->SetScale({ 0.1f,0.1f,0.4f });
	object0->SetRotation(rotation0);
	object0->Update();

	object1->SetPosition({ 0,-5,0 });
	object1->SetScale({ 1.0f,0.01f,1.0f });
	object1->SetRotation({ 0.0f,0.0f,0.0f });
	object1->Update();

	object2->SetPosition({0, -3, 0});
	object2->SetScale({ 0.01f,0.01f,0.01f });
	object2->SetRotation({ 0,0,0 });
	object2->Update();

	//スプライト更新
	sprite0->SetAlpha(1.0f);
	sprite0->SetScale({ 100.0f, 100.0 });
	sprite0->SetPosition({ 0.0f, 0.0 });
	sprite0->Update();

	sprite1->SetAlpha(1.0f);
	sprite1->SetScale({ 100.0f, 100.0 });
	sprite1->SetPosition({ 100.0f, 0.0 });
	sprite1->Update();

	sprite2->SetAlpha(1.0f);
	sprite2->SetScale({ 100.0f, 100.0 });
	sprite2->SetPosition({ 200.0f, 0.0 });
	sprite2->Update();
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
	//object0->Draw0(dxCommon_->GetCommandList());
	object2->Draw0(dxCommon_->GetCommandList());
	object1->Draw0(dxCommon_->GetCommandList());
}

void GameScene::Draw1()
{
	//object0->Draw1(dxCommon_->GetCommandList());
	object2->Draw1(dxCommon_->GetCommandList());
	object1->Draw1(dxCommon_->GetCommandList());
}

void GameScene::Draw2()
{
	//object0->Draw2(dxCommon_->GetCommandList());
	object2->Draw2(dxCommon_->GetCommandList());
	object1->Draw2(dxCommon_->GetCommandList());

	sprite0->Draw(dxCommon_->GetCommandList());
	sprite1->Draw(dxCommon_->GetCommandList());
	sprite2->Draw(dxCommon_->GetCommandList());
}

void GameScene::SetSRV(ID3D12DescriptorHeap* SRV)
{
	object0->SetSRV(SRV);
	object1->SetSRV(SRV);
	object2->SetSRV(SRV);
}

DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}
