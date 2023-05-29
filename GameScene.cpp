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
	camera_->SetEye({ 0, 0,-20 });

	//���C�g(�e)
	light = new Light;
	light->Initialize();

	//���C�g
	LightGroup::StaticInitialize(dxCommon_->GetDevice());
	lightGroup = LightGroup::Create();

	//�{�����[�����C�g
	VolumeLightModel* newLightModel = new VolumeLightModel();
	newLightModel->CreateBuffers(dxCommon_->GetDevice());
	volumeLightModel.reset(newLightModel);
	volumeLightModel->SetImageData(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	VolumeLightObject::SetDevice(dxCommon_->GetDevice());
	VolumeLightObject::SetCamera(camera_.get());
	VolumeLightObject::SetInput(input);
	VolumeLightObject::CreateGraphicsPipeline();

	VolumeLightObject* newLightObject = new VolumeLightObject();
	newLightObject->Initialize();
	newLightObject->SetModel(volumeLightModel.get());
	volumeLightObject.reset(newLightObject);

	//FBX�ǂݍ���
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//���f�������w�肵�ăt�@�C���ǂݍ���
	modelStone = FbxLoader::GetInstance()->LoadModelFromFile("Stone", "Resources/white1x1.png");
	modelTree0 = FbxLoader::GetInstance()->LoadModelFromFile("Tree", "Resources/white1x1.png");
	modelTree1 = FbxLoader::GetInstance()->LoadModelFromFile("Tree1", "Resources/black.png");
	modelTree2 = FbxLoader::GetInstance()->LoadModelFromFile("Tree2", "Resources/white1x1.png");
	modelTree3 = FbxLoader::GetInstance()->LoadModelFromFile("Tree3", "Resources/black.png");
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("Cube", "Resources/toriko.png");
	model2 = FbxLoader::GetInstance()->LoadModelFromFile("Walking", "Resources/white1x1.png");

	//�f�o�C�X���Z�b�g
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLight(light);
	FbxObject3D::SetLightGroup(lightGroup);
	FbxObject3D::CreateGraphicsPipelineLightView();
	FbxObject3D::CreateGraphicsPipeline();

	//�t�@�C���ǂݍ���
	tree1csv = new CSVLoader;
	tree1csv->LoadCSV("Resources/Tree1.csv");

	//���x���G�f�B�^
	jsonLoader = new JSONLoader();
	jsonLoader->LoadFile("Resources/untitled.json");

	for (int i = 0; i < objectNum; i++)
	{
		std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
		newObject->Initialize();

		if (jsonLoader->GetFileName(i) == "Cube")
		{
			newObject->SetModel(model1);
		}

		newObject->SetPosition(jsonLoader->GetPosition(i));
		newObject->SetScale(jsonLoader->GetScale(i));
		newObject->SetRotation(jsonLoader->GetRotation(i));

		object.push_back(std::move(newObject));
	}

	//�L���[�u

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
	camera_->SetTarget({ 0.0f,0.0f,0.0f });
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

	//�{�����[�����C�g
	volumeLightModel->Update();
	volumeLightObject->SetPosition(XMFLOAT3(volumeLightPos));
	volumeLightObject->SetRotation(XMFLOAT3(volumeLightRotation));
	volumeLightObject->SetScale(XMFLOAT3(volumeLightScale));
	volumeLightObject->Update();

	//���x���G�f�B�^
	jsonLoader = new JSONLoader();
	jsonLoader->LoadFile("Resources/untitled.json");

	//�X�y�[�X�L�[�Ńt�@�C���ǂݍ��ݍX�V
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
	//Tree1

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
