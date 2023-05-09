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
	modelStone = FbxLoader::GetInstance()->LoadModelFromFile("Tree", "Resources/white1x1.png");
	modelTree = FbxLoader::GetInstance()->LoadModelFromFile("Tree", "Resources/white1x1.png");
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/grassFiled.png");
	model2 = FbxLoader::GetInstance()->LoadModelFromFile("Walking", "Resources/white1x1.png");

	//�f�o�C�X���Z�b�g
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLight(light);
	FbxObject3D::SetLightGroup(lightGroup);
	FbxObject3D::CreateGraphicsPipelineLightView();
	FbxObject3D::CreateGraphicsPipeline();

	//�I�u�W�F�N�g������
	//��
	for (int i = 0; i < verticalStoneNum; i++)
	{
		for (int j = 0; j < horizonStoneNum; j++)
		{
			std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
			newObject->Initialize();
			newObject->SetModel(modelStone);

			newObject->SetPosition({ j * horizonStoneWidth - (horizonStoneWidth * horizonStoneNum) / 2, 0.0f,
				i * verticalStoneWidth - (verticalStoneWidth * verticalStoneNum) / 2 });
			newObject->SetScale(treeScale);
			newObject->SetRotation(treeRotation);

			objectStone.push_back(std::move(newObject));
		}
	}

	//��
	objectTree = new FbxObject3D;
	objectTree->Initialize();
	objectTree->SetModel(modelTree);
	objectTree->SetPosition(treePosition);
	objectTree->SetRotation(treeRotation);
	objectTree->SetScale(treeScale);

	//�L���[�u
	object1 = new FbxObject3D;
	object1->Initialize();
	object1->SetModel(model1);

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

	//�{�����[�����C�g
	volumeLightModel->Update();
	volumeLightObject->SetPosition(XMFLOAT3(volumeLightPos));
	volumeLightObject->SetRotation(XMFLOAT3(volumeLightRotation));
	volumeLightObject->SetScale(XMFLOAT3(volumeLightScale));
	volumeLightObject->Update();

	//�I�u�W�F�N�g�X�V
	for (std::unique_ptr<FbxObject3D>& object : objectStone)
	{
		object->SetScale(treeScale);
		object->SetRotation(treeRotation);
		object->Update();
	}

	//��
	treeRotation.y += 0.02f;
	objectTree->SetRotation(treeRotation);
	objectTree->Update();

	//�I�u�W�F�N�g�X�V
	object1->SetPosition({ -3,0,4 });
	object1->SetScale({ 1.0f,0.001f,1.0f });
	object1->SetRotation({0,0,0});
	object1->Update();
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
	volumeLightObject->Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawFBXLightView()
{
	for (std::unique_ptr<FbxObject3D>& object : objectStone)
	{
		object->DrawLightView(dxCommon_->GetCommandList());
	}
	object1->DrawLightView(dxCommon_->GetCommandList());
	objectTree->DrawLightView(dxCommon_->GetCommandList());
}

void GameScene::DrawFBX()
{
	for (std::unique_ptr<FbxObject3D>& object : objectStone)
	{
		object->Draw(dxCommon_->GetCommandList());
	}
	object1->Draw(dxCommon_->GetCommandList());
	objectTree->Draw(dxCommon_->GetCommandList());
}

void GameScene::SetSRV(ID3D12DescriptorHeap* SRV)
{
	for (std::unique_ptr<FbxObject3D>& object : objectStone)
	{
		object->SetSRV(SRV);
	}
	objectTree->SetSRV(SRV);
	object1->SetSRV(SRV);
}

DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}
