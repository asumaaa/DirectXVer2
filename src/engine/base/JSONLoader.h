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

public:	//サブクラス
	struct ObjectData {
		// ファイル名
		std::string fileName;
		//オブジェクトの名前
		std::string objectName;
		// 平行移動
		DirectX::XMFLOAT3 position;
		// 回転角
		DirectX::XMFLOAT3 rotation;
		// スケーリング
		DirectX::XMFLOAT3 scale;
	};

	struct ColliderData {
		// コライダーの種類
		std::string type;
		//オブジェクトの名前
		std::string objectName;
		// スケーリング
		DirectX::XMFLOAT3 scale;
		// 回転角
		DirectX::XMFLOAT3 rotation;
		//中心
		DirectX::XMFLOAT3 center;
	};

	struct TextureData{
		//テクスチャ1
		int textureNum1;
		//テクスチャ2
		int textureNum2;
		//テクスチャ3
		int textureNum3;
		//テクスチャ4
		int textureNum4;
		//シェーダ名
		std::string shaderName;
		//テクスチャの枚数
		int textureVol;
	};

	//メンバ関数
public:
	void LoadFile(const std::string fileName);

	//ゲッター
	ObjectData GetObjectData(int number) { return objectData[number]; }
	ColliderData GetColliderData(int number) { return colliderData[number]; }
	TextureData GetTextureData(int number) { return textureData[number]; }
	std::string GetFileName(int number) { return objectData[number].fileName; }
	XMFLOAT3 GetPosition(int number) { return objectData[number].position; }
	XMFLOAT3 GetRotation(int number) { return objectData[number].rotation; }
	XMFLOAT3 GetScale(int number){ return objectData[number].scale; }
	size_t GetObjectNum() { return objectNum; }

	//メンバ変数
private:
	std::vector<ObjectData>objectData;
	std::vector<ColliderData>colliderData;
	std::vector<TextureData>textureData;
	size_t objectNum = 0.0f;
};

