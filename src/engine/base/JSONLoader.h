/**
 * @file JSONLoader.h
 * @brief JSONファイルをゲームで使う形で読み込む
 * @author Asuma Syota
 * @date 2023/4
 */

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

	struct TextureData {
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
		//シェーダ名
		std::string shaderName2;
		//テクスチャの枚数
		int textureVol;
		//シェーダの数
		int shaderVol = 1;
	};

	//敵の行動パターン
	struct EnemyPatern {
		//パターンの名前
		std::string status;
		//パターンの番号
		int statusNum;
		//そのパターンの時間
		int time;
		//次のパターンの名前
		std::string nextStatus;
		//次のパターンの番号
		int nextStatusNum;
	};

	//メンバ関数
public:
	void LoadFile(const std::string fileName);

	std::vector<EnemyPatern> LoadEnemyPatern(const std::string fileName);

	/// <summary>
	///オブジェクトデータ取得
	/// </summary>
	ObjectData GetObjectData(int number) { return objectData[number]; }

	/// <summary>
	///コライダーデータ取得
	/// </summary>
	ColliderData GetColliderData(int number) { return colliderData[number]; }

	/// <summary>
	///テクスチャデータ取得
	/// </summary>
	TextureData GetTextureData(int number) { return textureData[number]; }

	/// <summary>
	///ファイルネーム取得
	/// </summary>
	std::string GetFileName(int number) { return objectData[number].fileName; }

	/// <summary>
	///座標取得
	/// </summary>
	XMFLOAT3 GetPosition(int number) { return objectData[number].position; }

	/// <summary>
	///角度取得
	/// </summary>
	XMFLOAT3 GetRotation(int number) { return objectData[number].rotation; }

	/// <summary>
	///スケール取得
	/// </summary>
	XMFLOAT3 GetScale(int number) { return objectData[number].scale; }

	/// <summary>
	///番号取得
	/// </summary>
	size_t GetObjectNum() { return objectNum; }

	//メンバ変数
private:
	std::vector<ObjectData>objectData;
	std::vector<ColliderData>colliderData;
	std::vector<TextureData>textureData;
	size_t objectNum = 0.0f;
};
