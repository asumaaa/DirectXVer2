#pragma once
#include "DirectXMath.h"

class CollisionPrimitive
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

public:	//サブクラス

	//球
	struct Sphere
	{
		//中心座標
		XMVECTOR center = { 0,0,0,1 };
		//半径
		float redius = 1.0f;
	};

	//平面
	struct Plane
	{
		//法線ベクトル
		XMVECTOR normal = { 0,1,0,0 };
		//原点からの距離
		float distance = 0.0f;
	};
};

