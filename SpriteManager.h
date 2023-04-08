#pragma once
#include "d3dx12.h"
#include "DirectXMath.h"

class SpriteManager
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//�����o�֐�
	void LoadFile(int number,const wchar_t* fileName);

public:	//�Q�b�^�[
	ID3D12Resource* GetTextureBuff() { return textureBuff.Get(); }
	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap.Get(); }

public:
	static void SetDevice(ID3D12Device* device) { SpriteManager::device = device; }

private:	//�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;

private:	//�����o�ϐ�
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource>textureBuff;
	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap;
};

