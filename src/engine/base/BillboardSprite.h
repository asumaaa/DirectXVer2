#pragma once
#include "BillboardSpriteModel.h"
#include "Camera.h"
#include "string.h"
#include "input.h"

class BillboardSprite
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		XMFLOAT4 color;
	};
public:	//�ÓI�����o�֐�
	//�Z�b�^�[
	static void SetDevice(ID3D12Device* device) { BillboardSprite::device = device; }
	static void SetCamera(Camera* camera) { BillboardSprite::camera = camera; }
	static void SetInput(Input* input) { BillboardSprite::input = input; }
	static void SetModel(BillboardSpriteModel* model) { BillboardSprite::model = model; }

private://�ÓI�����o�ϐ�
	static ID3D12Device* device;
	static Camera* camera;
	static Input* input;
	static BillboardSpriteModel* model;

public://�����o�֐�
	//������
	void Initialize();
	//�X�V
	void Update();
	void UpdateBillboard();
	//�ړ�
	void Move();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�O���t�B�b�N�X�p�C�v���C���̐���
	static void CreateGraphicsPipeline();
	//�Z�b�^�[
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetScale(XMFLOAT3 sca) { scale = sca; }
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }
	void SetColor(XMFLOAT4 color) { this->color = color; }
	//�Q�b�^�[
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }

private://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuffTransform;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelinestate;

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
	XMMATRIX matBillboard = XMMatrixIdentity();
	//�F
	XMFLOAT4 color = { 1,1,1,1 };
};

