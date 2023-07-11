#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"

class PlayerBullet
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	//�����o�֐�
public:
	//�ÓI�����o�֐�
	static void SetCamera(Camera* camera) { PlayerBullet::camera = camera; }
	static void SetInput(Input* input) { PlayerBullet::input = input; }
	static void SetDXInput(DXInput* dxInput) { PlayerBullet::dxInput = dxInput; }
	static void SetModel(FbxModel* model) { PlayerBullet::model = model; }

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//�ړ�
	void Move();
	//�V���b�g�t���O���^�Ȃ�Βe����
	void CreateBullet();
	//�e���폜���鏈��
	void DeleteBullet();

	//�Z�b�^�[
	void SetSRV(ID3D12DescriptorHeap* SRV);
	//�V���b�g����t���O���󂯎��
	void SetShotFlag(bool shotFlag) {PlayerBullet::shotFlag = shotFlag; };
	//�e�ɕK�v�ȏ����Z�b�g����
	void SetBullet(XMFLOAT3 position, XMFLOAT3 velocity);
	//�q�b�g�t���O���Z�b�g����
	void SetHitFlag(bool hitFlag, int num) { PlayerBullet::hitFlag[num] = hitFlag; };

	//�Q�b�^�[
	/*XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }*/
	//�R���C�_�[�f�[�^
	JSONLoader::ColliderData GetColliderData(int num);
	//�e�̐�
	size_t GetBulletNum() { return object.size(); }
	//���W
	XMFLOAT3 GetPosition(int num) { return position[num]; }

	//�ÓI�����o�ϐ�
private:
	//�J����
	static Camera* camera;
	//�L�[�{�[�h
	static Input* input;
	//�R���g���[���[
	static DXInput* dxInput;
	//���f��
	static FbxModel* model;

	//�����o�ϐ�
public:

	//�I�u�W�F�N�g
	std::list<std::unique_ptr<FbxObject3D>>object;

	//���W
	std::vector<XMFLOAT3> position;
	//��]
	std::vector<XMFLOAT3> rotation;
	//�T�C�Y
	std::vector<XMFLOAT3> scale;
	//�i�s�x�N�g��
	std::vector<XMFLOAT3> velocity;
	//�^�C�}�[
	std::vector<float>timer;
	//�t���O
	std::vector<bool>hitFlag;

	XMFLOAT3 baseRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 baseScale = { 0.3f,0.3f,0.3f };
	//�e�������鑬�x
	float destoryTime= 120.0f;

	//�e�ɂ���ԍ�
	int number = 0;

	//�V���b�g�t���O
	bool shotFlag = false;

	//�X�s�[�h
	float posSpeed = 1.0f;
};

