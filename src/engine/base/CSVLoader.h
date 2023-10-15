/**
 * @file CSVLoader.h
 * @brief CSVファイルをゲームで使う形で読み込む
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once
#include "array"
#include "DirectXMath.h"
#include "vector"
#include "string"

class CSVLoader
{
private:	//エイリアス
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	/// <summary>
	///csvファイルを読み込んで変数に代入する
	/// </summary>
	void LoadCSV(const std::string fileName);

	/// <summary>
	///座標取得
	/// </summary>
	XMFLOAT3 GetPosition(int num) { return position[num]; };

	/// <summary>
	///角度取得
	/// </summary>
	XMFLOAT3 GetRotation(int num) { return rotation[num]; };

	/// <summary>
	///スケール取得
	/// </summary>
	XMFLOAT3 GetScale(int num) { return scale[num]; };

private:
	//読み込むオブジェクトの数
	int objectNum = 0;

	std::vector<XMFLOAT3> position;
	std::vector<XMFLOAT3> rotation;
	std::vector<XMFLOAT3> scale;
};