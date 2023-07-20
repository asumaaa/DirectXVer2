#pragma once
#include "string"
#include "vector"
#include "DirectXMath.h"
#include "DirectXTex.h"
#include "windows.h"
#include "wrl.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "fbxsdk.h"
#include "TextureManager.h"

struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};

class FbxModel
{
public:
	//�萔
	//�{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;
	//�t�����h�N���X
	friend class FbxLoader;
	//�T�u�N���X
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;	//���W
		DirectX::XMFLOAT3 normal;	//�@���x�N�g��
		DirectX::XMFLOAT2 uv;	//uv���W
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};
	//�{�[���\����
	struct Bone
	{
		//���O
		std::string name;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�N���X�^�[
		FbxCluster* fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin>vertices;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short>indices;

private:
	//�t�@�C���̖��O
	std::string fileName;
	///���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;
	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	//�f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };

private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScracthImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template<class T>using vector = std::vector<T>;

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

private:	//�ÓI�����o�ϐ�
	//�e�N�X�`���}�l�[�W���[
	static TextureManager* textureManager;
	//�f�o�C�X
	static ID3D12Device* device;

public:
	//�R���X�g���N�^
	FbxModel(){};
	//�f�X�g���N�^
	~FbxModel();
	//�e�N�X�`���}�l�[�W���[�Z�b�g
	static void SetTextureManager(TextureManager* textureManager) { FbxModel::textureManager = textureManager; }
	//�f�o�C�X�Z�b�g
	static void SetDevice(ID3D12Device* device) { FbxModel::device = device; }
	//�o�b�t�@����
	void CreateBuffers();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList, int textureNum);
	//�|�X�g�G�t�F�N�g�p
	//�e�N�X�`��1���̏ꍇ
	void DrawTexture1(ID3D12GraphicsCommandList* cmdList, int textureNum);	
	//�e�N�X�`��2���̏ꍇ
	void DrawTexture2(ID3D12GraphicsCommandList* cmdList, int textureNum1, int textureNum2);
	//�e�N�X�`��3���̏ꍇ
	void DrawTexture3(ID3D12GraphicsCommandList* cmdList, int textureNum1, int textureNum2, int textureNum3);
	//�e�N�X�`��4���̏ꍇ
	void DrawTexture4(ID3D12GraphicsCommandList* cmdList,int textureNum1,int textureNum2, int textureNum3, int textureNum4);
	void PreDraw(ID3D12GraphicsCommandList* cmdList);
	//���f���̕ό`�s��̃Q�b�^�[
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	std::string GetFileName() { return fileName; }

	ComPtr<ID3D12Resource> GetTexBuff() { return texBuff; }

private:
	//�{�[���z��
	std::vector<Bone>bones;
public:
	//getter
	std::vector<Bone>& GetBones() { return bones; }

private:
	//FBX�V�[��
	FbxScene* fbxScene = nullptr;
public:
	//getter
	FbxScene* GetFbxScene() { return fbxScene; }
};

