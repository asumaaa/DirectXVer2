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
	static const int textureNum = 25;

public:	//メンバ関数
	void SetTexture(const wchar_t* fileName);
	void SetSprite(Sprite* sprite);
	void Initialize();
	void Update();
	void Draw();

private:	//メンバ変数
	//スプライト
	std::list < std::unique_ptr<Sprite>> sprite;
};

