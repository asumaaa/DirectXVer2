#pragma once
#include "Sprite.h"

class FireParticle
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//定数
	//テクスチャの数
	static const int spriteNum = 25;
	static const int textureNum = 2;

public:	//メンバ関数
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:	//セッター
	//使うスプライトの番号を指定
	void SetTextureNum(int num0, int num1) { textureNum0 = num0; textureNum1 = num1;}
	void SetPosition(XMFLOAT3 position) { FireParticle::position = position; }
	void SetRotation(XMFLOAT3 rotation) { FireParticle::rotation = rotation; }
	void SetScale(XMFLOAT3 scale) { FireParticle::scale = scale; }

private:	//メンバ変数
	//座標
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 scale = { 0,0,0 };
	XMFLOAT3 rotation = { 0,0,0 };

	//使用するテクスチャの番号
	int textureNum0 = 0;
	int textureNum1 = 1;

	//スプライト
	std::list < std::unique_ptr<Sprite>> sprite;
	//各スプライトの変形行列
	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> scales;
	std::vector<XMFLOAT3> rotations;
	//各ベクトル
	std::vector<XMFLOAT2> velocitys;
	std::vector<XMFLOAT3> colors;

	//タイマー
	float timer = 0;
	float time = spriteNum;
};

