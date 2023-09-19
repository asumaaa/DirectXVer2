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
	static const int spriteNum = 25;
	static const int textureNum = 2;

public:	//�����o�֐�
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:	//�Z�b�^�[
	//�g���X�v���C�g�̔ԍ����w��
	void SetTextureNum(int num0, int num1) { textureNum0 = num0; textureNum1 = num1;}
	void SetPosition(XMFLOAT3 position) { FireParticle::position = position; }
	void SetRotation(XMFLOAT3 rotation) { FireParticle::rotation = rotation; }
	void SetScale(XMFLOAT3 scale) { FireParticle::scale = scale; }

private:	//�����o�ϐ�
	//���W
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 scale = { 0,0,0 };
	XMFLOAT3 rotation = { 0,0,0 };

	//�g�p����e�N�X�`���̔ԍ�
	int textureNum0 = 0;
	int textureNum1 = 1;

	//�X�v���C�g
	std::list < std::unique_ptr<Sprite>> sprite;
	//�e�X�v���C�g�̕ό`�s��
	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> scales;
	std::vector<XMFLOAT3> rotations;
	//�e�x�N�g��
	std::vector<XMFLOAT2> velocitys;
	std::vector<XMFLOAT3> colors;

	//�^�C�}�[
	float timer = 0;
	float time = spriteNum;
};

