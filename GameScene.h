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
	FbxModel* modelTree = nullptr;
	std::list<std::unique_ptr<FbxObject3D>> objectTree;
	//�؂̐�
	size_t verticalTreeNum = 3;
	size_t horizonTreeNum = 6;
	//�؂̕�
	float verticalTreeWidth = 10.0f;
	float horizonTreeWidth = 9.0f;
	size_t treeNum = verticalTreeNum * horizonTreeNum;
	//�؂̕ό`�s��
	XMFLOAT3 treeScale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 treeRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 treePosition = { 0.0f,0.0f,0.0f };

	//��
	FbxModel* model1 = nullptr;
	FbxObject3D* object1 = nullptr;
	FbxModel* model2 = nullptr;

	//���C�g
	Light* light = nullptr;

	//�X�v���C�g�}�l�[�W���[
	SpriteManager* spriteManager = nullptr;

	//�ό`�s��
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };
};
