#pragma once
#include "d3dx12.h"
#include "DirectXMath.h"
#include "array"

class Sprite
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//�T�u�N���X
	//�萔�o�b�t�@
	struct ConstBuffMaterial
	{
		XMFLOAT4 color;
	};
	//�萔�o�b�t�@2
	struct ConstBuffTransform
	{
		XMMATRIX mat;	//3D�ϊ��s��
	};
	//���_�f�[�^�p�\����
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

public:	 //�萔
	//SRV�̍ő��
	static const size_t kMaxSrvCount = 2056;

public:	//�����o�֐�
	void Initialize();
	void Update();
	void LoadFile(const wchar_t* fileName);
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:	//�ÓI�����o�ϐ��Z�b�^�[
	static void SetDevice(ID3D12Device* device) { Sprite::device = device; }
	static void CreateGraphicsPipeLine();

public:	//�Z�b�^�[
	//�A���t�@�l
	void SetAlpha(float alpha) { color.w = alpha; }
	//���W
	void SetPosition(XMFLOAT2 pos) { position = pos; }
	//�p�x
	void SetRotation(float rot) { rotation = rot; }
	//�X�P�[��
	void SetScale(XMFLOAT2 sca) { scale = sca; }

public:	//�Q�b�^�[
	//���W
	XMFLOAT2 GetPosition() { return position; }
	//�p�x
	float GetRotation() { return rotation; }
	//�X�P�[��
	XMFLOAT2 GetScale() { return scale; }

private:
	//�f�o�C�X
	static ID3D12Device* device;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelinestate;

private:	//�����o�ϐ�
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//���_�f�[�^
	Vertex vertices[6];
	Vertex* vertMap = nullptr;
	//�萔�o�b�t�@ �}�e���A��
	ComPtr<ID3D12Resource>constBuffMaterial;
	ConstBuffMaterial* constMapMaterial = nullptr;
	//�萔�o�b�t�@ �ό`�s��
	ComPtr<ID3D12Resource>constBuffTransform;
	ConstBuffTransform* constMapTransform = nullptr;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource>textureBuff;
	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	//�e�N�X�`���̐F
	XMFLOAT4 color = {1,1,1,1};

private:
	float rotation = 0;
	XMFLOAT2 position = { 0,0 };
	XMFLOAT2 scale = { 100.0f,100.0f };
};
