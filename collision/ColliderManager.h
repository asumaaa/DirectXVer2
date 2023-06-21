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
	void SetCollider(JSONLoader::ColliderData colliderData);
	void SetColliderCubeModel(ColliderCubeModel* colliderModel) { ColliderManager::colliderCubeModel = colliderModel; }
	void SetColliderSphereModel(ColliderSphereModel* colliderModel) { ColliderManager::colliderSphereModel = colliderModel; };
	void SetColliderPlaneModel(ColliderPlaneModel* colliderModel) { ColliderManager::colliderPlaneModel = colliderModel; };

public:	//�����o�֐�
	//������
	void Initialize();
	//�X�V
	void PreUpdate();
	void PostUpdate();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//����
	bool CheckCollider(JSONLoader::ColliderData colliderData0, JSONLoader::ColliderData colliderData1);
	//���̂Ƌ���
	bool CheckSphereSphere(JSONLoader::ColliderData colliderSphere0, JSONLoader::ColliderData colliderSphere1);
	//���ʂƋ���
	bool CheckPlaneSphere(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderSphere);
	//���ʂƃ{�b�N�X
	bool CheckPlaneBox(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderBox);

	//ColliderData�N���X����OBB��Ԃ��֐�	Box,���ʂ̂ݑΉ�
	OBB GetObbFromColliderData(JSONLoader::ColliderData colliderData);
	//�F��ς���֐�
	void ChangeHitColor(JSONLoader::ColliderData colliderData);

private:	//�����o�֐�
	//���f��
	ColliderCubeModel*colliderCubeModel;
	ColliderSphereModel* colliderSphereModel;
	ColliderPlaneModel*colliderPlaneModel;
	//�R���C�_�[
	std::list<std::unique_ptr<Collider>>collider;

	//�������Ă��Ȃ��Ƃ��̐F
	XMFLOAT4 noHitColor = { 0,0,1,1 };
	XMFLOAT4 isHitColor = { 0,1,0,1 };
};

