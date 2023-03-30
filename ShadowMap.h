#pragma once

#include "DirectXMath.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "vector"
#include "WinApp.h"

class ShadowMap
{
private: // �G�C���A�X 
	// Microsoft::WRL���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//�T�u�N���X
	struct ConstBufferDate
	{
		XMMATRIX lightViewProjection;
		XMFLOAT4 lightColor;
		XMFLOAT3 lightDir;
	};

public:	//�����o�֐�
	//�f�o�C�X���Z�b�g
	static void SetDevice(ID3D12Device* device) { ShadowMap::device = device; }
	//���C�g�̍��W���Z�b�g
	inline void SetLightPos(const XMFLOAT3& lightPos, const XMFLOAT3& target, const XMFLOAT3& up);
	//���C�g�̕������Z�b�g
	inline void SetLightDir(XMFLOAT3 lightDir) { this->lightDir = lightDir; }

public:
	void CreateBuffer();
	void Update();
	void Draw();

private://�ÓI�����o�ϐ�
	static ID3D12Device* device;

private:	//�����o�ϐ�
	//���C�g�̍��W
	XMFLOAT3 lightPos;
	//���C�g
	XMMATRIX lightViewProjection;
	XMMATRIX lightView;
	XMFLOAT4 lightColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT3 lightDir;
};

