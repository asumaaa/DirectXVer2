/**
 * @file PostEffect.h
 * @brief �|�X�g�G�t�F�N�g�̊��N���X 
 * @author Asuma Syota
 * @date 2024/2
 */

#pragma once
#include "d3dx12.h"
#include "array"
#include "DirectXMath.h"
#include "DirectXTex.h"
#include "vector"
class PostEffect
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	 //�萔
	//SRV�̍ő��
	static const size_t kMaxSrvCount = 2056;

public:	//�T�u�N���X
	//�萔�o�b�t�@
	struct ConstBuffMaterial
	{
		//�F
		XMFLOAT4 color;
		//���E�̉𑜓x
		XMFLOAT2 strengthWeight;
		//�E�B���h�E
		XMFLOAT2 window;
		float pad1;
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

public:	//�����o�֐�

	/// <summary>
	///������
	/// </summary>
	void Initialize();

	/// <summary>
	///�X�V
	/// </summary>
	void Update();

	/// <summary>
	///�`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	///�`��O����
	/// </summary>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///�`��㏈��
	/// </summary>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

public:	//�ÓI�����o�֐�

	/// <summary>
	///�f�o�C�X�Z�b�g
	/// </summary>
	static void SetDevice(ID3D12Device* device) { PostEffect::device = device; }

public:	//�Z�b�^�[

	/// <summary>
	///�A���t�@�l�Z�b�g
	/// </summary>
	void SetAlpha(float alpha) { color.w = alpha; }

	/// <summary>
	///�F�Z�b�g
	/// </summary>
	void SetColor(XMFLOAT3 c) { color.x = c.x; color.y = c.y; color.z = c.z; }

	/// <summary>
	///���W�Z�b�g
	/// </summary>
	void SetPosition(XMFLOAT2 pos) { position = pos; }

	/// <summary>
	///�p�x�Z�b�g
	/// </summary>
	void SetRotation(float rot) { rotation = rot; }

	/// <summary>
	///�X�P�[���Z�b�g
	/// </summary>
	void SetScale(XMFLOAT2 sca) { scale = sca; }

	/// <summary>
	///�u���[�̋����Z�b�g
	/// </summary>
	void SetWidthStrength(float re) { widthStrength = re; }

	/// <summary>
	///�u���[�̋����Z�b�g
	/// </summary>
	void SetHeightStrength(float re) { heightStrength = re; }

protected:	//�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelinestate;
	//��ʃN���A�J���[
	static const float clearColor[4];

protected:	//�����o�ϐ�
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
	//�e�N�X�`���̐F
	XMFLOAT4 color = { 1,1,1,1 };
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource>textureBuff;
	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource>depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>descHeapDSV;

protected:
	//�𑜓x
	float widthStrength = 100;
	float heightStrength = 10;
	float rotation = 0;
	XMFLOAT2 position = { 0,0 };
	XMFLOAT2 scale = { 100.0f,100.0f };
};

