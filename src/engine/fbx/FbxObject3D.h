/**
 * @file FbxObject3D.h
 * @brief FBXの描画を行う
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once

#include "FbxModel.h"
#include "Camera.h"

#include "windows.h"
#include "wrl.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "DirectXMath.h"
#include "string.h"
#include "Light.h"
#include "LightGroup.h"
#include "JSONLoader.h"

class FbxObject3D
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
	//定数
	static const int MAX_BONES = 320;
	//サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		XMMATRIX lightviewproj;
		float timer1;
		float timer2;
	};
	//定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:	//静的メンバ関数

	/// <summary>
	///デバイスセット
	/// </summary>
	static void SetDevice(ID3D12Device* device) { FbxObject3D::device = device; }

	/// <summary>
	///カメラセット
	/// </summary>
	static void SetCamera(Camera* camera) { FbxObject3D::camera = camera; }

	/// <summary>
	///ライト(影用)セット
	/// </summary>
	static void SetLight(Light* light) { FbxObject3D::light = light; }

	/// <summary>
	///ライトセット
	/// </summary>
	static void SetLightGroup(LightGroup* lightGroup) { FbxObject3D::lightGroup = lightGroup; }

private://静的メンバ変数
	static ID3D12Device* device;
	static Camera* camera;
	static Light* light;
	static LightGroup* lightGroup;

public://メンバ関数

	/// <summary>
	///初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///初期化
	/// </summary>
	void Initialize(FbxModel* model,JSONLoader::TextureData textureData);

	/// <summary>
	///更新
	/// </summary>
	void Update();

	/// <summary>
	///ビルボード行列更新
	/// </summary>
	void UpdateBillboard();

	/// <summary>
	///コライダー更新
	/// </summary>
	void UpdateCollider();

	/// <summary>
	///タイマー更新
	/// </summary>
	void UpdateTimer();

	/// <summary>
	///ライト視点での描画
	/// </summary>
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///モデルセット
	/// </summary>
	void SetModel(FbxModel* model) { this->model = model; }

	/// <summary>
	///ライト視点のグラフィックスパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipelineLightView();

	/// <summary>
	///影付きカメラ視点のグラフィックスパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	///マルチテクスチャ用グラフィックスパイプラインの生成
	/// </summary>
	void CreateGraphicsPipelineTexture1();	//テクスチャ1枚の場合
	void CreateGraphicsPipelineTexture2();	//テクスチャ2枚の場合
	void CreateGraphicsPipelineTexture3();	//テクスチャ3枚の場合
	void CreateGraphicsPipelineTexture4();	//テクスチャ4枚の場合

	void CreateGraphicsPipelineShader2Texture2();	//シェーダ2テクスチャ2枚の場合

	/// <summary>
	///アニメーション開始
	/// </summary>
	void PlayAnimation();

	/// <summary>
	///アニメーション終了
	/// </summary>
	void StopAnimation() { isPlay = false; }

	/// <summary>
	///座標取得
	/// </summary>
	void SetPosition(XMFLOAT3 pos) { position = pos; }

	/// <summary>
	///角度取得
	/// </summary>
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }

	/// <summary>
	///スケール取得
	/// </summary>
	void SetScale(XMFLOAT3 sca) { scale = sca; }

	/// <summary>
	///srv取得
	/// </summary>
	void SetSRV(ID3D12DescriptorHeap* SRV) { depthSRV = SRV; }

	/// <summary>
	///オブジェクトデータ取得
	/// </summary>
	void SetObjectData(JSONLoader::ObjectData objectData);

	/// <summary>
	///コライダーデータ取得
	/// </summary>
	void SetColliderData(JSONLoader::ColliderData colliderData);

	/// <summary>
	///テクスチャデータ取得
	/// </summary>
	void SetTextureData(JSONLoader::TextureData textureData);

	/// <summary>
	///ビルボード行列を使うかのフラグ取得
	/// </summary>
	void SetBillboardFlag() { billboardFlag = true; }

	/// <summary>
	///テクスチャの番号取得
	/// </summary>
	void SetTextureNum(int textureNum) { this->textureNum1 = textureNum; }

	/// <summary>
	///外部から描画に使うシェーダの番号を指定
	/// </summary>
	void SetDrawShaderNum(int num) { drawShaderNum = num; }

	/// <summary>
	///座標取得
	/// </summary>
	XMFLOAT3 GetPosition() { return position; }

	/// <summary>
	///角度取得
	/// </summary>
	XMFLOAT3 GetRotation() { return rotation; }

	/// <summary>
	///スケール取得
	/// </summary>
	XMFLOAT3 GetScale() { return scale; }

	/// <summary>
	///ファイルネーム取得
	/// </summary>
	std::string GetFileName() { return fileName; }

	/// <summary>
	///オブジェクトネーム取得
	/// </summary>
	std::string GetObjectName() { return objectName; }

	/// <summary>
	///コライダーデータ取得
	/// </summary>
	JSONLoader::ColliderData GetColliderData() { return colliderData; }

private://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>constBuffTransform;
	//ライト視点のルートシグネチャとパイプライン(shadowMap用)
	static ComPtr<ID3D12RootSignature>rootsignature0;
	static ComPtr<ID3D12PipelineState>pipelinestate0;
	//シェーダを指定する場合のルートシグネチャとパイプライン
	ComPtr<ID3D12RootSignature>rootsignature1;
	ComPtr<ID3D12PipelineState>pipelinestate1;
	//影付きカメラ視点のルートシグネチャとパイプライン
	static ComPtr<ID3D12RootSignature>rootsignature2;
	static ComPtr<ID3D12PipelineState>pipelinestate2;
	//シェーダ2つめのルートシグネチャとパイプライン
	ComPtr<ID3D12RootSignature>rootsignature3;
	ComPtr<ID3D12PipelineState>pipelinestate3;

private:
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z軸回りのローカル行列
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//ビルボード行列
	XMMATRIX matBillboard;
	//Y軸周りビルボード行列
	XMMATRIX matBillboiadY;
	//モデル
	FbxModel* model = nullptr;
	//コライダーの中心と座標の差分
	XMFLOAT3 colliderPos0 = { 0.0f,0.0f,0.0f };
	XMFLOAT3 colliderPos1 = { 0.0f,0.0f,0.0f };
	XMFLOAT3 colliderCenter = { 0.0f,0.0f,0.0f };
	XMFLOAT3 colliderRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 colliderScale = { 0.0f,0.0f,0.0f };

	//ビルボードフラグ
	bool billboardFlag = false;

	//定数バッファ
	ComPtr<ID3D12Resource>constBuffSkin;

	//外部から受け取るSRV
	ID3D12DescriptorHeap* depthSRV;

	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;

	//ファイルの名前
	std::string fileName;
	//オブジェクトの名前
	std::string objectName;

	//コライダー
	JSONLoader::ColliderData colliderData;

	//テクスチャの番号
	int textureNum1 = 0;
	int textureNum2 = 0;
	int textureNum3 = 0;
	int textureNum4 = 0;
	//テクスチャの枚数
	int textureVol = 1;
	//使用するシェーダの名前
	std::string shaderName;
	std::string shaderName2;
	//シェーダの数
	int shaderVol = 1;
	bool shaderFlag = false;

	//何番目のシェーダで描画を行うか
	int drawShaderNum = 0;

	//タイマーシェーダ用1
	float timer1 = 0.0f;
	float fTime1 = 0.002f;
	float maxTime1 = 1.0f;

	//タイマーシェーダ用2
	float timer2 = 0.6f;
	float maxTime2 = 1.0f;
	float minTime2 = 0.1f;
	float fTime2 = 0.02f;
};