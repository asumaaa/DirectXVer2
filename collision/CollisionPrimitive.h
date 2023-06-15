#pragma once
#include "DirectXMath.h"

class CollisionPrimitive
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	/*template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;*/
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:	//�T�u�N���X

	//��
	struct Sphere
	{
		//���S���W
		XMVECTOR center = { 0,0,0,1 };
		//���a
		float redius = 1.0f;
	};

	//����
	struct Plane
	{
		//�@���x�N�g��
		XMVECTOR normal = { 0,1,0,0 };
		//���_����̋���
		float distance = 0.0f;
	};
};

