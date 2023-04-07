#pragma once
#include "Sprite.h"

class FireParticle
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//�萔
	//�e�N�X�`���̐�
	static const int textureNum = 25;

public:	//�����o�֐�
	void SetTexture(const wchar_t* fileName);
	void SetSprite(Sprite* sprite);
	void Initialize();
	void Update();
	void Draw();

private:	//�����o�ϐ�
	//�X�v���C�g
	std::list < std::unique_ptr<Sprite>> sprite;
};

