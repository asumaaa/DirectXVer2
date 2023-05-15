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
#include "FbxObject3DDemo.h"

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
	//CG5�p��
	FbxModel* modelDemo0 = nullptr;
	FbxObject3DDemo* objectDemo0 = nullptr;
	XMFLOAT3 demo0Position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 demo0Rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 demo0Scale = { 1.0f,1.0f,1.0f };
	//���C�g�̐F
	float lightColor[3] = { 1.0f,1.0f,0.0f };

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

	//�X�v���C�g�}�l�[�W���[
	SpriteManager* spriteManager = nullptr;

	//�ό`�s��
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };
};
