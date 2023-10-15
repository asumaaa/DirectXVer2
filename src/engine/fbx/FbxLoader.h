/**
 * @file FbxLoader.h
 * @brief FBXファイルをDirectXの描画用に変換する
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once

#include "fbxsdk.h"
#include "FbxModel.h"
#include "string"

#include <d3d12.h>
#include <d3dx12.h>

class FbxLoader
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

public:

	/// <summary>
	///初期化
	/// </summary>
	void Initialize(ID3D12Device* device);

	/// <summary>
	///終了時処理
	/// </summary>
	void Finalize();

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

private:
	//デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャー
	FbxManager* fbxManager = nullptr;
	//FBXインポーター
	FbxImporter* fbxImporter = nullptr;

private:
	using string = std::string;	//stdを省略

	//定数
public:
	//モデル格納ルートパス
	static const string baseDirectory;
	//テクスチャない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

	/// <summary>
	///FBXファイルの読み込み
	/// </summary>
	FbxModel* LoadModelFromFile(const string modelName);

	/// <summary>
	///ノード構成を解析
	/// </summary>
	void ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode,Node* parent = nullptr);

	/// <summary>
	///メッシュ解析
	/// </summary>
	void ParseMesh(FbxModel* model, FbxNode* fbxNode);

	/// <summary>
	///頂点座標読み取り
	/// </summary>
	void ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);

	/// <summary>
	///面情報読み取り
	/// </summary>
	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);

	/// <summary>
	///マテリアル読み取り
	/// </summary>
	void ParseMaterial(FbxModel* model, FbxNode* fbxNode);

	/// <summary>
	///テクスチャ読み取り
	/// </summary>
	void LoadTexture(FbxModel* model, const std::string& fullpath);

	/// <summary>
	///ディレクトリを含んだファイルパスからファイル名を抽出する
	/// </summary>
	std::string ExtractFileName(const std::string& path);

	/// <summary>
	///FBXの行列をXMMATRIXに変換
	/// </summary>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst,const FbxAMatrix& src);

	/// <summary>
	///スキニング情報読み取り
	/// </summary>
	void ParseSkin(FbxModel* model, FbxMesh* fbxMesh);

private:
	string textureName;
};