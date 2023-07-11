#pragma once
#include "ColliderCubeObject.h"
#include "ColliderSphereObject.h"
#include "ColliderPlaneObject.h"
#include "JSONLoader.h"

class ColliderManager
{
public:	//�T�u�N���X
	//�R���C�_�[�`��p
	struct Collider 
	{
		JSONLoader::ColliderData colliderData;
		ColliderCubeObject* colliderCubeObject = nullptr;
		ColliderSphereObject* colliderSphereObject = nullptr;
		ColliderPlaneObject* colliderPlaneObject = nullptr;
		//�`��t���O
		bool drawFlag = false;
	};

	//OBB ����v�Z�p
	struct OBB
	{
		XMFLOAT3 c;	//���S�_
		XMFLOAT3 u[3];	//XYZ�̊e���W���̌X����\���x�N�g��
		XMFLOAT3 e;	//OBB�̊e���W���ɉ����������̔���
	};

public:	//�ÓI�����o�֐�
	static void SetCollider(JSONLoader::ColliderData colliderData);
	static void SetColliderCubeModel(ColliderCubeModel* colliderModel) { ColliderManager::colliderCubeModel = colliderModel; }
	static void SetColliderSphereModel(ColliderSphereModel* colliderModel) { ColliderManager::colliderSphereModel = colliderModel; };
	static void SetColliderPlaneModel(ColliderPlaneModel* colliderModel) { ColliderManager::colliderPlaneModel = colliderModel; };

public:	//�����o�֐�
	//������
	static void Initialize();
	//�X�V
	static void PreUpdate();
	static void PostUpdate();
	//�`��
	static void Draw(ID3D12GraphicsCommandList* cmdList);

	//����
	static bool CheckCollider(JSONLoader::ColliderData colliderData0, JSONLoader::ColliderData colliderData1);
	//���̂Ƌ���
	static bool CheckSphereSphere(JSONLoader::ColliderData colliderSphere0, JSONLoader::ColliderData colliderSphere1);
	//���ʂƋ���
	static bool CheckPlaneSphere(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderSphere);
	//���ʂƃ{�b�N�X
	static bool CheckPlaneBox(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderBox);

	//ColliderData�N���X����OBB��Ԃ��֐�	Box,���ʂ̂ݑΉ�
	OBB GetObbFromColliderData(JSONLoader::ColliderData colliderData);
	//�F��ς���֐�
	static void ChangeHitColor(JSONLoader::ColliderData colliderData);

public:	//�ÓI�����o�ϐ�
	//�R���C�_�[
	static std::list<std::unique_ptr<Collider>>collider;
	//���f��
	static ColliderCubeModel* colliderCubeModel;
	static ColliderSphereModel* colliderSphereModel;
	static ColliderPlaneModel* colliderPlaneModel;

	//�������Ă��Ȃ��Ƃ��̐F
	static XMFLOAT4 noHitColor;
	static XMFLOAT4 isHitColor;
};

