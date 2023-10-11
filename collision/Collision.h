#pragma once
#include "CollisionPrimitive.h"

class Collision
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	/*template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;*/
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	//球と平面当たり判定
	static bool CheckSphere2Plane(const CollisionPrimitive::Sphere& sphere,
		const CollisionPrimitive::Plane& plane, XMVECTOR* inter = nullptr);
};
