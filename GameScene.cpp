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

	//FBX読み込み
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	model0 = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/white1x1.png");

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::CreateGraphicsPipeline();

	//オブジェクト初期化
	object0 = new FbxObject3D;
	object0->Initialize();
	object0->SetModel(model0);
}

void GameScene::Update()
{
	//カメラ更新
	camera_->DebugUpdate();
	camera_->Update();
	//コントローラー更新
	dxInput->InputProcess();

	//オブジェクト更新
	object0->SetPosition(position);
	object0->SetScale(scale);
	object0->SetRotation(rotation);
	object0->Update();

}

void GameScene::Draw()
{
	object0->Draw(dxCommon_->GetCommandList());
}
