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
	camera_->SetEye({ 0, 10,-10 });

	//ライト生成
	lightGroup = LightGroup::Create();

	/*lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);*/
	lightGroup->SetPointLightActive(0, true);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);
	/*lightGroup->SetSpotLightActive(0, false);
	lightGroup->SetSpotLightActive(1, false);
	lightGroup->SetSpotLightActive(2, false);*/
	lightGroup->SetCircleShadowActive(0, true);

	//FBX読み込み
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	model0 = FbxLoader::GetInstance()->LoadModelFromFile("key", "Resources/key.png");
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/white1x1.png");
	model2 = FbxLoader::GetInstance()->LoadModelFromFile("light", "Resources/white1x1.png");

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLightGroup(lightGroup);
	FbxObject3D::CreateGraphicsPipeline();

	//オブジェクト初期化
	object0 = new FbxObject3D;
	object0->Initialize();
	object0->SetModel(model0);

	object2 = new FbxObject3D;
	object2->Initialize();
	object2->SetModel(model2);

	object1 = new FbxObject3D;
	object1->Initialize();
	object1->SetModel(model1);
	
}

void GameScene::Update()
{
	//カメラ更新
	camera_->DebugUpdate();
	camera_->Update();
	//コントローラー更新
	dxInput->InputProcess();

	//ライト更新
	/*lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0],lightDir0[1], lightDir0[2],0 }));
	lightGroup->SetDirLightColor(0,XMFLOAT3(lightColor0));*/

	lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos0));
	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor0));
	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten0));

	/*lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0],circleShadowDir[1], circleShadowDir[2],0 }));
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3({0,1,0}));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));*/

	lightGroup->Update();

	//オブジェクト更新
	rotation0.y += 0.02;
	object0->SetPosition({0,1,0});
	object0->SetScale({0.2f,0.1f,0.4f});
	object0->SetRotation(rotation0);
	object0->Update();

	object2->SetPosition(XMFLOAT3(pointLightPos0));
	object2->SetScale({ 0.2f,0.1f,0.4f });
	object2->SetRotation({0,0,0});
	object2->Update();

	object1->SetPosition({0,0,0});
	object1->SetScale({0.2,0.001,0.2});
	object1->SetRotation(rotation1);
	object1->Update();

}

void GameScene::Draw()
{
	ImGui::Begin("Light");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 500));
	/*ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir0",lightDir0);
	ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("circleShadowDir", circleShadowDir);*/
	ImGui::ColorEdit3("pointLightColor", pointLightColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("pointLightPos", pointLightPos0);
	ImGui::InputFloat3("pointLightAtten", pointLightAtten0);
	ImGui::End();

	object0->Draw(dxCommon_->GetCommandList());
	object1->Draw(dxCommon_->GetCommandList());
	object2->Draw(dxCommon_->GetCommandList());
}
