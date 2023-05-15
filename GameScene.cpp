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

	//�J����������
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 10,-20 });

	//���C�g(�e)
	light = new Light;
	light->Initialize();

	//���C�g
	LightGroup::StaticInitialize(dxCommon_->GetDevice());
	lightGroup = LightGroup::Create();

	//FBX�ǂݍ���
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//���f�������w�肵�ăt�@�C���ǂݍ���
	modelDemo0 = FbxLoader::GetInstance()->LoadModelFromFile("SpherePBR", "Resources/toriko.png");

	//�f�o�C�X���Z�b�g
	FbxObject3DDemo::SetDevice(dxCommon_->GetDevice());
	FbxObject3DDemo::SetCamera(camera_.get());
	FbxObject3DDemo::SetLight(light);
	FbxObject3DDemo::SetLightGroup(lightGroup);
	FbxObject3DDemo::CreateGraphicsPipeline();

	//��
	objectDemo0 = new FbxObject3DDemo;
	objectDemo0->Initialize();
	objectDemo0->SetModel(modelDemo0);
	objectDemo0->SetPosition(demo0Position);
	objectDemo0->SetRotation(demo0Rotation);
	objectDemo0->SetScale(demo0Scale);

	//�X�v���C�g�}�l�[�W���[
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
	//�J�����X�V
	camera_->SetEye({ 0.0f,10.0f,5.0f });
	camera_->DebugUpdate();
	camera_->Update();
	//�R���g���[���[�X�V
	dxInput->InputProcess();

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

	//��
	objectDemo0->SetColor(XMFLOAT3(lightColor));
	objectDemo0->SetRotation(demo0Rotation);
	objectDemo0->SetPosition(demo0Position);
	objectDemo0->SetScale(demo0Scale);
	objectDemo0->Update();
}

void GameScene::Draw()
{
	ImGui::Begin("Light");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::ColorEdit3("LightColor", lightColor);
	ImGui::End();

	DrawFBX();
}

void GameScene::DrawFBXLightView()
{
}

void GameScene::DrawFBX()
{
	objectDemo0->Draw(dxCommon_->GetCommandList());
}

void GameScene::SetSRV(ID3D12DescriptorHeap* SRV)
{
	/*objectTree->SetSRV(SRV);*/
}

DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}
