#pragma once
#include "memory"
#include "list"

#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "FireParticle.h"
#include "Light.h"
#include "LightGroup.h"
#include "VolumeLightModel.h"
#include "VolumeLightObject.h"
#include "CSVLoader.h"

#define PI 3.1415

class GameScene
{
	//�����o�֐�
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//�X�V
	void Update();
	//�`��
	void Draw();
	void DrawFBXLightView();
	void DrawFBX();
	
	//�Z�b�^�[
	void SetSRV(ID3D12DescriptorHeap* SRV);
	//�Q�b�^�[
	DirectX::XMMATRIX GetLightViewProjection();

	//�����o�ϐ�
private:
	//�f�o�C�X��input
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	//�J����
	std::unique_ptr<Camera> camera_;

	//fbx
	//��
	FbxModel* modelStone = nullptr;
	std::list<std::unique_ptr<FbxObject3D>> objectStone;
	//�΂̐�
	size_t verticalStoneNum = 10;
	size_t horizonStoneNum = 10;
	//�΂̕�
	float verticalStoneWidth = 10.0f;
	float horizonStoneWidth = 9.0f;
	size_t treeNum = verticalStoneNum * horizonStoneNum;
	//�΂̕ό`�s��
	XMFLOAT3 stoneScale = { 2.0f,2.0f,2.0f };
	XMFLOAT3 stoneRotation = { 1.5f * PI,0.0f,0.0f };
	XMFLOAT3 stonePosition = { 0.0f,0.0f,0.0f };

	//�؂̃��f��
	FbxModel* modelTree0 = nullptr;
	FbxModel* modelTree1 = nullptr;
	FbxModel* modelTree2 = nullptr;
	FbxModel* modelTree3 = nullptr;
	//�؂̃I�u�W�F�N�g
	std::list<std::unique_ptr<FbxObject3D>> objectTree0;
	std::list<std::unique_ptr<FbxObject3D>> objectTree1;
	std::list<std::unique_ptr<FbxObject3D>> objectTree2;
	//�؂̐�
	size_t tree0Num = 5;
	size_t tree1Num = 10;
	size_t tree2Num = 5;
	//�؂̕ό`�s��
	XMFLOAT3 tree0Position = { 0.0f,2.0f,5.0f };
	XMFLOAT3 tree0Rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 tree0Scale = {5.0f,5.0f,5.0f };

	CSVLoader *tree1csv = nullptr;

	//��
	FbxModel* model1 = nullptr;
	FbxObject3D* object1 = nullptr;
	FbxObject3D* object2 = nullptr;
	FbxModel* model2 = nullptr;

	//���C�g �e�p
	Light* light = nullptr;
	float lightDir[3] = {0.0f,-1.0f , -1.0f};
	float lightPos[3] = {0.0f,25.0f,25.0f};
	float lightTarget[3] = { 0.0f,0.0f,0.0f };
	float lightFactorAngle[2] = { 20.0f,30.0f, };
	float lightAtten[3] = {0.0f,0.0f,0.0f};

	//���C�g
	LightGroup* lightGroup = nullptr;
	float lightManagerDir[3] = { 0.0f,-1.0f , 1.0f };

	//�{�����[�����C�g
	std::unique_ptr <VolumeLightModel> volumeLightModel;
	std::unique_ptr <VolumeLightObject> volumeLightObject;
	float volumeLightPos[3] = { 0.0f,5.0f,-10.0f };
	float volumeLightScale[3] = { 4.0f,4.0f,4.0f };
	float volumeLightRotation[3] = { 0.0f,0.0f,0.0f };

	//�X�v���C�g�}�l�[�W���[
	SpriteManager* spriteManager = nullptr;

	//�ό`�s��
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };
};
