#pragma once
#include "d3dx12.h"
#include "DirectXMath.h"

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
	struct ConstBuff
	{
		XMFLOAT4 color;
	};
	//���_�f�[�^�p�\����
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

public:	//�����o�֐�
	void Initialize();
	void LoadFile(const wchar_t* fileName);
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:	//�ÓI�����o�ϐ��Z�b�^�[
	static void SetDevice(ID3D12Device* device) { Sprite::device = device; }
	static void CreateGraphicsPipeLine();

public:
	void SetAlpha(float alpha) { color.w = alpha; }

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
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuffMaterial;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource>textureBuff;
	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	//�e�N�X�`���̐F
	XMFLOAT4 color = {1,1,1,1};
};

