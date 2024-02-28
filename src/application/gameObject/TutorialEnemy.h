/**
 * @file TutorialEnemy.h
 * @brief �Q�[���I�u�W�F�N�g �G
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"
#include "Sprite.h"
#include "JSONLoader.h"

class TutorialEnemy
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�����o�֐�

	/// <summary>
	///�J�����Z�b�g
	/// </summary>
	static void SetCamera(Camera* camera) { TutorialEnemy::camera = camera; }

	/// <summary>
	///���̓Z�b�g
	/// </summary>
	static void SetInput(Input* input) { TutorialEnemy::input = input; }

	/// <summary>
	/// �C���X�g���N�^ �f�X�g���N�^
	/// </summary>
	TutorialEnemy() {};
	~TutorialEnemy() {};

	/// <summary>
	///������
	/// </summary>
	void Initialize();

	/// <summary>
	///�X�V
	/// </summary>
	void Update();

	/// <summary>
	///�I�u�W�F�N�g�X�V
	/// </summary>
	void UpdateObject();

	/// <summary>
	///�X�v���C�g�X�V
	/// </summary>
	void UpdateSprite();

	/// <summary>
	///�R���C�_�[
	/// </summary>
	void UpdateCollider();

	/// <summary>
	///�_���[�W�n�S��
	/// </summary>
	void UpdateDamage();

	/// <summary>
	///�`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///���C�g���_�`��
	/// </summary>
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///�X�v���C�g�`��
	/// </summary>
	void DrawSprite(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///�����S��
	/// </summary>
	void Move();

	/// <summary>
	///�X�e�[�^�X�}�l�[�W���[
	/// </summary>
	void StatusManager();

	/// <summary>
	///srv�Z�b�g
	/// </summary>
	void SetSRV(ID3D12DescriptorHeap* SRV);

	/// <summary>
	///���Z�b�g
	/// </summary>
	void Reset();

	/// <summary>
	///���@�̒e��e��
	/// </summary>
	void HitBullet1();

	/// <summary>
	///���W�擾
	/// </summary>
	XMFLOAT3 GetPosition() { return position; }

	/// <summary>
	///�p�x�擾
	/// </summary>
	XMFLOAT3 GetRotation() { return rotation; }

	/// <summary>
	///�X�P�[���擾
	/// </summary>
	XMFLOAT3 GetScale() { return scale; }

	/// <summary>
	///�R���C�_�[�擾
	/// </summary>
	JSONLoader::ColliderData GetColliderData() { return colliderData; }

	/// <summary>
	///���S�t���O�擾
	/// </summary>
	bool GetIsDead() { return isDead; }

	//�ÓI�����o�ϐ�
private:
	//�J����
	static Camera* camera;
	//�L�[�{�[�h
	static Input* input;

	//�����o�ϐ�
public:

	//�R���C�_�[�f�[�^
	JSONLoader::ColliderData colliderData;

	//�I�u�W�F�N�g
	FbxObject3D* object = nullptr;
	//���f��
	FbxModel* model= nullptr;

	//�X�v���C�g
	Sprite* hpBar1 = nullptr;	//HP�o�[�̘g
	Sprite* hpBar2 = nullptr;	//HP�o�[
	Sprite* hpBar3 = nullptr;	//���݂�HP�̂Ƃ��ɂ�����
	Sprite* hpBar4 = nullptr;	//BOSS HP�̃e�L�X�g

	//�ό`�s��
	//���s�ړ�
	XMFLOAT3 position = { 0.0f,0.0f,30.0f };
	//��]
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	//�T�C�Y
	XMFLOAT3 scale = { 5.0f,5.0f,5.0f };

	//HP
	float maxHP = 30;
	float HP = maxHP;

	//�X�v���C�g�p
	//HP�o�[1
	XMFLOAT2 hpBar1Pos = { 320.0f,-15.0f };
	XMFLOAT2 hpBar1Scale = { 640.0f,96.0f };
	//HP�o�[2
	XMFLOAT2 hpBar2Pos = { 349.0f,26.0f };
	XMFLOAT2 hpBar2OriginalScale = { 605.0f,15.0f };
	XMFLOAT2 hpBar2Scale = hpBar2OriginalScale;
	//HP�o�[3
	XMFLOAT2 hpBar3OriginalPos = { 938.0f,25.0f };
	XMFLOAT2 hpBar3Pos = hpBar3OriginalPos;
	XMFLOAT2 hpBar3Scale = { 16.0f,16.0f };
	//Hp�o�[4
	XMFLOAT2 hpBar4Pos = { 230.0f,25.0f };
	XMFLOAT2 hpBar4Scale = { 96.0f,16.0f };

	//��e
	bool HitFlag1 = false;

	//���S�t���O
	bool isDead = false;
};