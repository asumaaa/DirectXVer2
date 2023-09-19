#pragma once
#include "CollisionPrimitive.h"

class Collision
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

public:

	//���ƕ��ʓ����蔻��
	static bool CheckSphere2Plane(const CollisionPrimitive::Sphere& sphere,
		const CollisionPrimitive::Plane& plane, XMVECTOR* inter = nullptr);
};

