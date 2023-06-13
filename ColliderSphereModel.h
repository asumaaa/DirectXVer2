#pragma once
#include "vector"
#include "DirectXMath.h"
#include "DirectXCommon.h"
#include "DirectXTex.h"
#include "windows.h"
#include "wrl.h"
#include "d3d12.h"
#include "d3dx12.h"

class ColliderSphereModel
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
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

public:
	//�o�b�t�@����
	void CreateBuffers(ID3D12Device* device);
	//���_����
	void CreateVertex();
	//�F�ݒ�
	void SetImageData(XMFLOAT4 color);
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
public:
	//���_�f�[�^�z��
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;	//���W
		XMFLOAT3 normal = XMFLOAT3(0, 0, 0);	//�@���x�N�g��
		XMFLOAT2 uv = XMFLOAT2(0, 0);	//uv���W

		/*VertexPosNormalUv(XMFLOAT3 p)
		{
			this->pos = p;
			this->normal = XMFLOAT3(0, 0, 0);
			this->uv = XMFLOAT2(0, 0);
		}*/
	};
	//���_�f�[�^�z��
	vector<VertexPosNormalUv>vertices;
	//���_�C���f�b�N�X�z��
	vector<unsigned short>indices;
private:
	//���̂̕����� ��������قǏd���Ȃ�
	size_t divNum = 8;
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
};

