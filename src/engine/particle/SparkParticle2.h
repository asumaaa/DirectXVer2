#pragma once

#include "TextureManager.h"
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
#include "forward_list"

class SparkParticle2
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
		XMMATRIX mat;
		XMMATRIX matBillboard;
	};

	//�p�[�e�B�N��1��
	struct Particle
	{
		//���W
		XMFLOAT3 position = {};
		//���x
		XMFLOAT3 velocity = {};
		//�����x
		XMFLOAT3 accel = {};
		//���݃t���[��
		int frame = 0;
		//�I���t���[��
		int num_frame = 0;
		//�X�P�[��
		float scale = 1.0f;
		//�����l
		float startScale = 10.0f;
		//�ŏI�n
		float endScale = 0.0f;
	};

	//���_�f�[�^�z��
	struct VertexPos
	{
		XMFLOAT3 pos;	//���W
		float scale;
		XMFLOAT3 velocity;
	};

public:	//�ÓI�����o�֐�
	static void SetSpriteManager(TextureManager* spriteManager) { SparkParticle2::spriteManager = spriteManager; };
	static void SetDevice(ID3D12Device* device) { SparkParticle2::device = device; }
	static void SetCamera(Camera* camera) { SparkParticle2::camera = camera; }
	static void SetInput(Input* input) { SparkParticle2::input = input; }
	//�O���t�B�b�N�X�p�C�v���C���̐���
	static void CreateGraphicsPipeline();

public:
	//�o�b�t�@����
	void CreateBuffers();
	//�X�V
	void Update();
	void UpdateParticle();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�p�[�e�B�N����ǉ�
	void Add(XMFLOAT3 pos);
	void AddParticle(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float startScale, float endScale);

	//�Z�b�^�[
	//�e�N�X�`���̔ԍ�
	void SetTextureNum(int num) { textureNum = num; }

public:	//�ÓI�����o�ϐ�
	static TextureManager* spriteManager;
	static ID3D12Device* device;
	static Camera* camera;
	static Input* input;
	//���_�ő吔
	static const int vertexCount = 1024;
	//�Ή�1��Ɏg�����_��
	static const int sparkCount = 128;

private:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuffTransform;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelinestate;

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource>texBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;
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

	//�p�[�e�B�N���z��
	std::forward_list<Particle>particles;
	//�e�N�X�`���̔ԍ�
	int textureNum = 0;
};

