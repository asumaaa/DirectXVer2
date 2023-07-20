#pragma once
#include "string"
#include "vector"
#include "DirectXMath.h"
#include "DirectXTex.h"
#include "windows.h"
#include "wrl.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "fbxsdk.h"
#include "TextureManager.h"

struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
public:
	//定数
	//ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;
	//フレンドクラス
	friend class FbxLoader;
	//サブクラス
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;	//座標
		DirectX::XMFLOAT3 normal;	//法線ベクトル
		DirectX::XMFLOAT2 uv;	//uv座標
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};
	//ボーン構造体
	struct Bone
	{
		//名前
		std::string name;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//クラスター
		FbxCluster* fbxCluster;
		//コンストラクタ
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

	//メッシュを持つノード
	Node* meshNode = nullptr;
	//頂点データ配列
	std::vector<VertexPosNormalUvSkin>vertices;
	//頂点インデックス配列
	std::vector<unsigned short>indices;

private:
	//ファイルの名前
	std::string fileName;
	///モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
	//アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	//ディフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };

private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScracthImage = DirectX::ScratchImage;
	//std::を省略
	using string = std::string;
	template<class T>using vector = std::vector<T>;

private:
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファ
	ComPtr<ID3D12Resource>indexBuff;
	//テクスチャバッファ
	ComPtr<ID3D12Resource>texBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;

private:	//静的メンバ変数
	//テクスチャマネージャー
	static TextureManager* textureManager;
	//デバイス
	static ID3D12Device* device;

public:
	//コンストラクタ
	FbxModel(){};
	//デストラクタ
	~FbxModel();
	//テクスチャマネージャーセット
	static void SetTextureManager(TextureManager* textureManager) { FbxModel::textureManager = textureManager; }
	//デバイスセット
	static void SetDevice(ID3D12Device* device) { FbxModel::device = device; }
	//バッファ生成
	void CreateBuffers();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList, int textureNum);
	//ポストエフェクト用
	//テクスチャ1枚の場合
	void DrawTexture1(ID3D12GraphicsCommandList* cmdList, int textureNum);	
	//テクスチャ2枚の場合
	void DrawTexture2(ID3D12GraphicsCommandList* cmdList, int textureNum1, int textureNum2);
	//テクスチャ3枚の場合
	void DrawTexture3(ID3D12GraphicsCommandList* cmdList, int textureNum1, int textureNum2, int textureNum3);
	//テクスチャ4枚の場合
	void DrawTexture4(ID3D12GraphicsCommandList* cmdList,int textureNum1,int textureNum2, int textureNum3, int textureNum4);
	void PreDraw(ID3D12GraphicsCommandList* cmdList);
	//モデルの変形行列のゲッター
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	std::string GetFileName() { return fileName; }

	ComPtr<ID3D12Resource> GetTexBuff() { return texBuff; }

private:
	//ボーン配列
	std::vector<Bone>bones;
public:
	//getter
	std::vector<Bone>& GetBones() { return bones; }

private:
	//FBXシーン
	FbxScene* fbxScene = nullptr;
public:
	//getter
	FbxScene* GetFbxScene() { return fbxScene; }
};

