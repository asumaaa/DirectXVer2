#pragma once
#include "ParticleModel.h"
#include "Camera.h"
#include "string.h"
#include "input.h"

class ParticleObject
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
	};
public:	//静的メンバ関数
	//セッター
	static void SetDevice(ID3D12Device* device) { ParticleObject::device = device; }
	static void SetCamera(Camera* camera) { ParticleObject::camera = camera; }
	static void SetInput(Input* input) { ParticleObject::input = input; }
	static void SetModel(ParticleModel* model) { ParticleObject::model = model; }

private://静的メンバ変数
	static ID3D12Device* device;
	static Camera* camera;
	static Input* input;
	static ParticleModel* model;

public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
	void UpdateBillboard();
	//移動
	void Move();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//グラフィックスパイプラインの生成
	static void CreateGraphicsPipeline();
	//セッター
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetScale(XMFLOAT3 sca) { scale = sca; }
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }
	/*void SetColor(XMFLOAT4 color) { this->color = color; }*/
	void SetTextureNum(int num) { textureNum = num; }
	//ゲッター
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }

private://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>constBuffTransform;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelinestate;
	//ビルボード行列
	XMMATRIX matBillboard;

private:
	//テクスチャの番号
	int textureNum = 0;
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z軸回りのローカル行列
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//色
	/*XMFLOAT4 color = { 1,1,1,1 };*/
};

