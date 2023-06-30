#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"
#include "Sprite.h"

class Enemy
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	//�����o�֐�
public:
	//�ÓI�����o�֐�
	static void SetCamera(Camera* camera) { Enemy::camera = camera; }
	static void SetInput(Input* input) { Enemy::input = input; }
	static void SetDXInput(DXInput* dxInput) { Enemy::dxInput = dxInput; }

	//������
	void Initialize();
	//�X�V
	void Update();
	void UpdateObject();
	void UpdateSprite();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);
	void DrawSprite(ID3D12GraphicsCommandList* cmdList);

	//�����֘A
	//�����S��
	void Move();
	//����
	void UpdateGravity();
	//�W�����v
	void UpdateJump();

	//�U���n�S��
	void UpdateAttack();

	//�Z�b�^�[
	void SetObject(FbxObject3D* object);
	void SetSRV(ID3D12DescriptorHeap* SRV);
	void HitPlane();

	//�Q�b�^�[
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }

	//�ÓI�����o�ϐ�
private:
	//�J����
	static Camera* camera;
	//�L�[�{�[�h
	static Input* input;
	//�R���g���[���[
	static DXInput* dxInput;

	//�����o�ϐ�
public:

	//�I�u�W�F�N�g
	std::unique_ptr<FbxObject3D>object;
	//�X�v���C�g
	std::unique_ptr<Sprite>spriteHpBar;

	//�ό`�s��
	//���s�ړ�
	XMFLOAT3 position = {0.0f,0.0f,0.0f};
	//��]
	XMFLOAT3 rotation = {0.0f,0.0f,0.0f};
	//�T�C�Y
	XMFLOAT3 scale = {1.0f,1.0f,1.0f};


	//�����蔻��֘A
	//�ڒn�t���O
	bool groundFlag = false;


	//�����֘A
	
	//����
	//�����x�N�g��
	XMFLOAT3 fallVelocity = { 0.0f,0.0f,0.0f };
	//�����^�C�}�[
	float fallTimer = 0.0f;
	//�����ő�l�܂łɂ����鎞��
	float fallTime = 1.0f;
	//1�t���[��������̗�����
	float fallFrame = 1.0f / 60.0f;

	//�W�����v
	float jumpHeight = 0.4;

	//�X�s�[�h
	float speed = 0.15f;

	//HP
	float HP = 100;
};

