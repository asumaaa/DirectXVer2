/**
 * @file EnemyBullet.h
 * @brief �Q�[���I�u�W�F�N�g �G�̒e
 * @author Asuma Syota
 * @date 2023/12
 */

#pragma once
#include "DirectXMath.h"
#include "Camera.h"
#include "JSONLoader.h"

class EnemyBullet
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:	//�T�u�N���X
	struct Bullet
	{
		//���W
		XMFLOAT3 position;
		//��]
		XMFLOAT3 rotation;
		//�T�C�Y
		XMFLOAT3 scale;
		//�i�s�x�N�g��
		XMFLOAT3 velocity;
		//�^�C�}�[
		float timer;
		//�t���O
		bool hitFlag;
		//�R���C�_�[�̃f�[�^
		JSONLoader::ColliderData colliderData;
	};

public:

	//�����o�֐�
public://�ÓI�����o�֐�

	/// <summary>
	///�f�o�C�X�Z�b�g
	/// </summary>
	static void SetCamera(Camera* camera) { EnemyBullet::camera = camera; }

	/// <summary>
	///������
	/// </summary>
	void Initialize();

	/// <summary>
	///�X�V
	/// </summary>
	void Update();

	/// <summary>
	///�R���C�_�[�X�V
	/// </summary>
	void UpdateCollider();

	/// <summary>
	///�`��
	/// </summary>
	void Draw();

	/// <summary>
	///�ړ�
	/// </summary>
	void Move();

	/// <summary>
	///�V���b�g�t���O���^�Ȃ�Βe����
	/// </summary>
	void CreateBullet();

	/// <summary>
	///�e���폜���鏈��
	/// </summary>
	void DeleteBullet();

	/// <summary>
	///�V���b�g����t���O���󂯎��
	/// </summary>
	void SetShotFlag(bool shotFlag) { EnemyBullet::shotFlag = shotFlag; };

	/// <summary>
	///�e�ɕK�v�ȏ����Z�b�g����
	/// </summary>
	void SetBullet(XMFLOAT3 position, XMFLOAT3 velocity);

	/// <summary>
	///�q�b�g�t���O���Z�b�g����
	/// </summary>
	void SetHitFlag(bool hitFlag, int num) { bullet[num].hitFlag = hitFlag; };

	/// <summary>
	///�R���C�_�[�f�[�^�擾
	/// </summary>
	JSONLoader::ColliderData GetColliderData(int num) { return bullet[num].colliderData; }

	/// <summary>
	///�e�̐��擾
	/// </summary>
	/*size_t GetBulletNum() { return object.size(); }*/

	/// <summary>
	///���W�擾
	/// </summary>
	XMFLOAT3 GetPosition(int num) { return bullet[num].position; }

	//�ÓI�����o�ϐ�
private:
	//�J����
	static Camera* camera;

	//�����o�ϐ�
private:

	//�I�u�W�F�N�g
	/*std::list<std::unique_ptr<FbxObject3D>>object;*/

	////���W
	//std::vector<XMFLOAT3> position;
	////��]
	//std::vector<XMFLOAT3> rotation;
	////�T�C�Y
	//std::vector<XMFLOAT3> scale;
	////�i�s�x�N�g��
	//std::vector<XMFLOAT3> velocity;
	////�^�C�}�[
	//std::vector<float>timer;
	////�t���O
	//std::vector<bool>hitFlag;

	//�e�̏��
	std::vector<Bullet> bullet;

	XMFLOAT3 baseRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 baseScale = { 0.3f,0.3f,0.3f };
	//�e�������鑬�x
	float destoryTime = 120.0f;

	//�e�ɂ���ԍ�
	int number = 0;

	//�V���b�g�t���O
	bool shotFlag = false;

	//�X�s�[�h
	float posSpeed = 1.0f;
};

