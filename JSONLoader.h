#pragma once
#include "array"
#include "DirectXMath.h"
#include "vector"
#include "memory"
#include "string"
#include "list"

class JSONLoader
{
private:	//エイリアス
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ObjectData {
		// ファイル名
		std::string fileName;
		// 平行移動
		XMFLOAT3 position;
		// 回転角
		XMFLOAT3 rotation;
		// スケーリング
		XMFLOAT3 scale;
	};

	//メンバ関数
public:
	void LoadFile(const std::string fileName);

	//ゲッター
	ObjectData GetObjectData(int number) { return objectData[number]; }
	std::string GetFileName(int number) { return objectData[number].fileName; }
	XMFLOAT3 GetPosition(int number) { return objectData[number].position; }
	XMFLOAT3 GetRotation(int number) { return objectData[number].rotation; }
	XMFLOAT3 GetScale(int number){ return objectData[number].scale; }

	//メンバ変数
private:
	std::vector<ObjectData>objectData;
};

