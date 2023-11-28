/**
 * @file DebugLineObject.h
 * @brief デバッグ用線分
 * @author Asuma Syota
 * @date 2023/11/8
 */

#pragma once
#include "DebugLineModel.h"
#include "Camera.h"
#include "string.h"
#include "input.h"

class DebugLineObject
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		XMFLOAT4 color;
	};
public:	//静的メンバ関数

	/// <summary>
	///デバイスセット
	/// </summary>
	static void SetDevice(ID3D12Device* device) { DebugLineObject::device = device; }

	/// <summary>
	///カメラセット
	/// </summary>
	static void SetCamera(Camera* camera) { DebugLineObject::camera = camera; }

	/// <summary>
	///入力セット
	/// </summary>
	static void SetInput(Input* input) { DebugLineObject::input = input; }

	/// <summary>
	///モデルセット
	/// </summary>
	static void SetModel(DebugLineModel* model) { DebugLineObject::model = model; }

private://静的メンバ変数
	static ID3D12Device* device;
	static Camera* camera;
	static Input* input;
	static DebugLineModel* model;

public://メンバ関数

	/// <summary>
	///初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///更新
	/// </summary>
	void Update();

	/// <summary>
	///移動
	/// </summary>
	void Move();

	/// <summary>
	///描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///パイプライン設定、作成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	///線分の原点設定
	/// </summary>
	void SetCenterPos(XMFLOAT3 pos) { position = pos; }

	/// <summary>
	///ベクトルセット
	/// </summary>
	void SetVector(XMFLOAT3 vec) { vector = vec; }

	/// <summary>
	///色セット
	/// </summary>
	void SetColor(XMFLOAT4 color) { this->color = color; }

	/// <summary>
	///座標取得
	/// </summary>
	XMFLOAT3 GetPosition() { return position; }

	/// <summary>
	///角度取得
	/// </summary>
	XMFLOAT3 GetRotation() { return rotation; }

private://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>constBuffTransform;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelinestate;

private:
	//X,Y,Z軸回りのローカル行列
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ベクトル
	XMFLOAT3 vector = { 1.0f,1.0f,1.0f };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//色
	XMFLOAT4 color = { 1,1,1,1 };
};
