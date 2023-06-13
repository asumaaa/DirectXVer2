#pragma once
#include "ColliderCubeModel.h"
#include "ColliderCubeObject.h"
#include "ColliderSphereModel.h"
#include "ColliderSphereObject.h"
#include "JSONLoader.h"

class ColliderManager
{
private:	//�T�u�N���X
	struct Collider 
	{
		JSONLoader::ColliderData colliderData;
		ColliderCubeObject* colliderCubeObject = nullptr;
		ColliderSphereObject* colliderSphereObject = nullptr;
		//�`��t���O
		bool drawFlag = false;
	};

public:	//�ÓI�����o�֐�
	void SetCollider(JSONLoader::ColliderData colliderData);
	void SetColliderCubeModel(ColliderCubeModel* colliderModel) { ColliderManager::colliderCubeModel = colliderModel; }
	void SetColliderSphereModel(ColliderSphereModel* colliderModel) { ColliderManager::colliderSphereModel = colliderModel; };

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

private:	//�����o�֐�
	//���f��
	ColliderCubeModel*colliderCubeModel;
	ColliderSphereModel*colliderSphereModel;
	//�I�u�W�F�N�g
	/*std::list<std::unique_ptr<ColliderCubeObject>>colliderCubeObject;
	std::list<std::unique_ptr<ColliderSphereObject>>colliderSphereObject;*/
	//�R���C�_�[
	std::list<std::unique_ptr<Collider>>collider;

	//�������Ă��Ȃ��Ƃ��̐F
	XMFLOAT4 noHitColor = { 0,0,1,1 };
};

