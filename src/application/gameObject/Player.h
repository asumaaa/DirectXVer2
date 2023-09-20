#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"
#include "PlayerBullet.h"

class Player
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	enum Status	//�v���C���[�̏��
	{
		Wait,
		Walk,
		Run
	};
	//�����o�֐�
public:
	//�ÓI�����o�֐�
	static void SetCamera(Camera* camera) { Player::camera = camera; }
	static void SetInput(Input* input) { Player::input = input; }
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }

	//������
	void Initialize();
	//�X�V
	void Update();
	void UpdateObject();
	void UpdateBullet();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//�����֘A
	//�����S��
	void Move();
	//�L�[����
	void KeyControl();
	//����
	void UpdateGravity();
	//�W�����v
	void UpdateJump();

	//�U���n�S��
	void UpdateAttack();

	//�Z�b�^�[
	void SetBullet(PlayerBullet* playerBullet) { Player::bullet = playerBullet; }
	void SetSRV(ID3D12DescriptorHeap* SRV);
	void HitPlane();

	//�Q�b�^�[
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation0() { return rotation0; }
	XMFLOAT3 GetRotation1() { return rotation1; }
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

	//�e
	PlayerBullet*bullet;

	//�҂��Ă��Ԃ̃I�u�W�F�N�g
	FbxObject3D* objectWait = nullptr;
	//�҂��Ă��Ԃ̃��f��
	FbxModel* modelWait = nullptr;

	//�����Ă��Ԃ̃I�u�W�F�N�g
	FbxObject3D* objectRun = nullptr;
	//�����Ă��Ԃ̃��f��
	FbxModel* modelRun = nullptr;

	//�ό`�s��
	//���s�ړ�
	XMFLOAT3 position = {0.0f,0.0f,0.0f};
	//��]
	XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	XMFLOAT3 rotation1 = {0.0f,0.0f,0.0f};
	//�T�C�Y
	XMFLOAT3 scale = {1.0f,1.0f,1.0f};
	//�i�s�x�N�g��
	XMFLOAT3 posVelocity = { 0.0f,0.0f,0.0f };
	//�p�x�x�N�g��
	XMFLOAT3 rotVelocity = { 0.0f,0.0f,0.0f };

	//�����蔻��֘A
	//�ڒn�t���O
	bool groundFlag = false;

	//�X�e�[�^�X�֘A
	//�v���C���[��HP
	float HP = 10;


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
	float posSpeed = 0.5f;
	float rotSpeed = (float)PI * 1.0f / 180.0f;

	//���
	Status status = Wait;
	//1�t���[���O�̏��
	Status preStatus = Wait;
};

