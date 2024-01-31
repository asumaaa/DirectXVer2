/**
 * @file ElecObject.cpp
 * @brief �d���̕`��̂��߂Ɏg�p����I�u�W�F�N�g
 * @author Asuma Syota
 * @date 2024/1
 */

#pragma once
#include "vector"
#include "DirectXMath.h"
#include "DirectXCommon.h"
#include "DirectXTex.h"
#include "windows.h"
#include "wrl.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "Camera.h"
#include "string.h"
#include "input.h"

#pragma once
class ElecObject
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
	using TexMetadata = DirectX::TexMetadata;
	using ScracthImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template<class T>using vector = std::vector<T>;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		XMFLOAT4 color;
	};

private://�ÓI�����o�ϐ�
	static ID3D12Device* device;
	static Camera* camera;
	static Input* input;

public:	//�ÓI�����o�֐�

	/// <summary>
	///�f�o�C�X�Z�b�g
	/// </summary>
	static void SetDevice(ID3D12Device* device) { ElecObject::device = device; }

	/// <summary>
	///�J�����Z�b�g
	/// </summary>
	static void SetCamera(Camera* camera) { ElecObject::camera = camera; }

	/// <summary>
	///���̓Z�b�g
	/// </summary>
	static void SetInput(Input* input) { ElecObject::input = input; }

	/// <summary>
	///�p�C�v���C���ݒ�A�쐬
	/// </summary>
	static void CreateGraphicsPipeline();

public:	//�����o�֐�

	/// <summary>
	///������
	/// </summary>
	void Initialize(XMFLOAT3 pos1,XMFLOAT3 pos2,float divNum);

	/// <summary>
	///�o�b�t�@�ݒ�A����
	/// </summary>
	void CreateBuffers(ID3D12Device* device);

	/// <summary>
	///���_�ݒ�A����
	/// </summary>
	void CreateVertex();

	/// <summary>
	///�F�ݒ�
	/// </summary>
	void SetImageData(XMFLOAT4 color);

	/// <summary>
	///�X�V
	/// </summary>
	void Update();

	/// <summary>
	///�`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:	//�����o�ϐ�
	//���_�f�[�^�z��
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;	//���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;	//uv���W
		VertexPosNormalUv* parent = nullptr;	//uv���W
	};
	//���_�f�[�^�z��
	vector<VertexPosNormalUv>vertices;
	//���_�C���f�b�N�X�z��
	vector<unsigned short>indices;
private:
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource>indexBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource>texBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuffTransform;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelinestate;
	//�A���r�G���g�W��
	XMFLOAT3 ambient = { 1,1,1 };
	//�f�B�t���[�Y�W��
	XMFLOAT3 diffuse = { 1,1,1 };
	//�e�N�X�`�����^�f�[�^
	TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	ScracthImage scratchImg = {};
	//�摜�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	const size_t textureWidth = 256;
	const size_t textureHeight = 256;
	const size_t imageDataCount = textureWidth * textureHeight;
	XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];
	//�e�N�X�`���[��GPU�̃n���h��
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	//���_�̐�
	size_t vertexValue = 24;
	//�C���f�b�N�X�̐�
	size_t indicesValue = 36;

	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z�����̃��[�J���s��
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//�F
	XMFLOAT4 color = { 1,1,1,1 };

	//�X�^�[�g�̍��W
	XMFLOAT3 startPosition = { 0.0f,0.0f,0.0f };
	//�����̍��W
};

