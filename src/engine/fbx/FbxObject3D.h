#pragma once

#include "FbxModel.h"
#include "Camera.h"

#include "windows.h"
#include "wrl.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "DirectXMath.h"
#include "string.h"
#include "Light.h"
#include "LightGroup.h"
#include "JSONLoader.h"

class FbxObject3D
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//�萔
	static const int MAX_BONES = 320;
	//�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		XMMATRIX lightviewproj;
	};
	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:	//�ÓI�����o�֐�
	//�Z�b�^�[
	static void SetDevice(ID3D12Device* device) { FbxObject3D::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3D::camera = camera; }
	static void SetLight(Light* light) { FbxObject3D::light = light; }
	static void SetLightGroup(LightGroup* lightGroup) { FbxObject3D::lightGroup = lightGroup; }

private://�ÓI�����o�ϐ�
	static ID3D12Device* device;
	static Camera* camera;
	static Light* light;
	static LightGroup* lightGroup;

public://�����o�֐�
	//������
	void Initialize();
	//�X�V
	void Update();
	void UpdateBillboard();
	void UpdateCollider();
	//�`��
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//���f���̃Z�b�g
	void SetModel(FbxModel* model) { this->model = model; }
	//�O���t�B�b�N�X�p�C�v���C���̐���
	static void CreateGraphicsPipelineLightView();
	static void CreateGraphicsPipeline();
	//�A�j���[�V�����J�n
	void PlayAnimation();

	//�Z�b�^�[
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 sca) { scale = sca; }
	void SetSRV(ID3D12DescriptorHeap* SRV) { depthSRV = SRV; }
	void SetObjectData(JSONLoader::ObjectData objectData);
	void SetColliderData(JSONLoader::ColliderData colliderData);
	void SetBillboardFlag() { billboardFlag = true; }

	//�Q�b�^�[
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }
	std::string GetFileName() { return fileName; }
	std::string GetObjectName() { return objectName; }
	JSONLoader::ColliderData GetColliderData() { return colliderData; }

private://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuffTransform;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature0;
	static ComPtr<ID3D12RootSignature>rootsignature2;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelinestate0;
	static ComPtr<ID3D12PipelineState>pipelinestate2;

private:
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z�����̃��[�J���s��
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//�r���{�[�h�s��
	XMMATRIX matBillboard;
	//Y������r���{�[�h�s��
	XMMATRIX matBillboiadY;
	//���f��
	FbxModel* model = nullptr;
	//�R���C�_�[�̒��S�ƍ��W�̍���
	XMFLOAT3 colliderPos = {0.0f,0.0f,0.0f};

	//�r���{�[�h�t���O
	bool billboardFlag = false;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuffSkin;

	//�O������󂯎��SRV
	ID3D12DescriptorHeap* depthSRV;

	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;

	//�t�@�C���̖��O
	std::string fileName;
	//�I�u�W�F�N�g�̖��O
	std::string objectName;

	//�R���C�_�[
	JSONLoader::ColliderData colliderData;
};