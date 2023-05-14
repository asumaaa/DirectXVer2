#pragma once
#include "d3dx12.h"
#include "array"
#include "DirectXMath.h"
#include "DirectXTex.h"
#include "vector"
#include "string"

class CSVLoader
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	void SetObjectNum(float num);
	void LoadCSV(const std::string textureName);

private:
	//読み込むオブジェクトの数
	int objectNum = 0;

	std::vector<XMFLOAT3> position;
	std::vector<XMFLOAT3> rotation;
	std::vector<XMFLOAT3> scale;
};

