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
		Run,
		BackRun,
		RunLeft,
		RunRight,
		Attack1,
		Attack2,
		Attack3,
	};
	//�����o�֐�
public:

	Player() {};
	~Player();

	//�ÓI�����o�֐�
	static void SetCamera(Camera* camera) { Player::camera = camera; }
	static void SetInput(Input* input) { Player::input = input; }
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }

	//������
	void Initialize();
	//�X�V
	void Update();
	void UpdateObject();
	void UpdateObject(Status status,FbxObject3D* object);
	void UpdateBullet();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//�����֘A
	//�����S��
	void Control();
	//�ړ�
	void Move();
	//����
	void UpdateGravity();
	//�W�����v
	void UpdateJump();

	//�X�e�[�^�X�}�l�[�W���[
	void StatusManager();

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

	//��둖�肵�Ă���v���C���[
	FbxObject3D* objectBackRun = nullptr;
	//��둖�肵�Ă郂�f��
	FbxModel* modelBackRun = nullptr;

	//���ɑ����Ă�I�u�W�F�N�g
	FbxObject3D* objectRunLeft = nullptr;
	//���ɑ����Ă��郂�f��
	FbxModel* modelRunLeft = nullptr;
	
	//�E�ɑ����Ă�I�u�W�F�N�g
	FbxObject3D* objectRunRight = nullptr;
	//�E�ɑ����Ă��郂�f��
	FbxModel* modelRunRight = nullptr;

	//�U��1�̃��f��
	FbxObject3D* objectAttack1 = nullptr;
	//�U��1�̃I�u�W�F�N�g
	FbxModel* modelAttack1 = nullptr;

	//�U��2�̃I�u�W�F�N�g
	FbxObject3D* objectAttack2 = nullptr;
	//�U��2�̃��f��
	FbxModel* modelAttack2 = nullptr;

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
	float posSpeed = 1.0f;
	float rotSpeed = (float)PI * 1.5f / 180.0f;

	//�U���֘A
	//Attack1�̃^�C�}�[
	float Attack1Time = 150.0f;
	float Attack1Timer = 0.0f;
	//Attack2�̃^�C�}�[
	float Attack2Time = 145.0f;
	float Attack2Timer = 0.0f;
	//�A���U���̓��̓t���[��
	float Attack1IntervalTime = 20.0f;

	//���
	Status status = Wait;
	//1�t���[���O�̏��
	Status preStatus = Wait;
};

