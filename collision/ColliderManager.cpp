#include "ColliderManager.h"
#include "math.h"
#include "mathOriginal.h"

using namespace DirectX;
#define PI 3.1415

std::list<std::unique_ptr<ColliderManager::Collider>>ColliderManager::collider;
ColliderCubeModel* ColliderManager::colliderCubeModel = nullptr;
ColliderSphereModel* ColliderManager::colliderSphereModel = nullptr;
ColliderPlaneModel* ColliderManager::colliderPlaneModel = nullptr;
XMFLOAT4 ColliderManager::noHitColor = { 0,0,1,1 };
XMFLOAT4 ColliderManager::isHitColor = { 0,1,0,1 };

void ColliderManager::SetCollider(JSONLoader::ColliderData colliderData)
{
	//�R���C�_�[�Z�b�g
	std::unique_ptr<Collider>newColliderData = std::make_unique<Collider>();
	newColliderData->colliderData.type = colliderData.type;
	newColliderData->colliderData.objectName = colliderData.objectName;
	newColliderData->colliderData.scale = colliderData.scale;
	newColliderData->colliderData.rotation = colliderData.rotation;
	newColliderData->colliderData.center = colliderData.center;

	//�I�u�W�F�N�g����
	//���̂̏ꍇ
	if (colliderData.type == "Sphere")
	{
		ColliderSphereObject* newCollider = new ColliderSphereObject();
		newCollider->Initialize();
		newColliderData->colliderSphereObject = newCollider;
	}
	//�{�b�N�X�̏ꍇ
	if (colliderData.type == "Box")
	{
		ColliderCubeObject* newCollider = new ColliderCubeObject();
		newCollider->Initialize();
		newColliderData->colliderCubeObject = newCollider;
	}
	//���ʂ̏ꍇ
	if (colliderData.type == "Plane")
	{
		ColliderPlaneObject* newCollider = new ColliderPlaneObject();
		newCollider->Initialize();
		newColliderData->colliderPlaneObject = newCollider;
	}

	collider.emplace_back(std::move(newColliderData));
}

void ColliderManager::Initialize()
{
}

void ColliderManager::PreUpdate()
{
	//�I�u�W�F�N�g�̍X�V
	for (std::unique_ptr<Collider>& colliders : collider)
	{
		/*colliders->drawFlag = false;*/
		colliders->drawFlag = false;
		if (colliders->colliderData.type == "Box")
		{
			colliders->colliderCubeObject->SetColor(noHitColor);
		}
		if (colliders->colliderData.type == "Sphere")
		{
			colliders->colliderSphereObject->SetColor(noHitColor);
		}
		if (colliders->colliderData.type == "Plane")
		{
			colliders->colliderPlaneObject->SetColor(noHitColor);
		}
	}
}

void ColliderManager::PostUpdate()
{
	//�I�u�W�F�N�g�̍X�V
	for (std::unique_ptr<Collider>& colliders : collider)
	{
		if (colliders->colliderData.type == "Box")
		{
			colliders->colliderCubeObject->SetPosition(colliders->colliderData.center);
			colliders->colliderCubeObject->SetRotation(colliders->colliderData.rotation);
			colliders->colliderCubeObject->SetScale(colliders->colliderData.scale);
			colliders->colliderCubeObject->Update();
		}
		if (colliders->colliderData.type == "Sphere")
		{
			colliders->colliderSphereObject->SetPosition(colliders->colliderData.center);
			colliders->colliderSphereObject->SetRotation(colliders->colliderData.rotation);
			colliders->colliderSphereObject->SetScale(colliders->colliderData.scale);
			colliders->colliderSphereObject->Update();
		}
		if (colliders->colliderData.type == "Plane")
		{
			colliders->colliderPlaneObject->SetPosition(colliders->colliderData.center);
			colliders->colliderPlaneObject->SetRotation(colliders->colliderData.rotation);
			colliders->colliderPlaneObject->SetScale(colliders->colliderData.scale);
			colliders->colliderPlaneObject->Update();
		}
	}
}

void ColliderManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (std::unique_ptr<Collider>& colliders : collider)
	{
		if (colliders->drawFlag == true)
		{
			if (colliders->colliderData.type == "Box")
			{
				colliders->colliderCubeObject->Draw(cmdList);
			}
			if (colliders->colliderData.type == "Sphere")
			{
				colliders->colliderSphereObject->Draw(cmdList);
			}
			if (colliders->colliderData.type == "Plane")
			{
				colliders->colliderPlaneObject->Draw(cmdList);
			}
		}
	}
}

bool ColliderManager::CheckCollider(JSONLoader::ColliderData colliderData0, JSONLoader::ColliderData colliderData1)
{

	for (std::unique_ptr<Collider>& colliders : collider)
	{
		//�`��t���O�𗧂Ă� �ʒu�X�V
		if (colliders->colliderData.objectName == colliderData0.objectName)
		{
			colliders->drawFlag = true;
			colliders->colliderData.scale = colliderData0.scale;
			colliders->colliderData.rotation = colliderData0.rotation;
			colliders->colliderData.center = colliderData0.center;
		}
		if (colliders->colliderData.objectName == colliderData1.objectName)
		{
			colliders->drawFlag = true;
			colliders->colliderData.scale = colliderData1.scale;
			colliders->colliderData.rotation = colliderData1.rotation;
			colliders->colliderData.center = colliderData1.center;
		}
	}

	//���Ƌ��̏ꍇ
	if (colliderData0.type == "Sphere" && colliderData1.type == "Sphere")
	{
		return CheckSphereSphere(colliderData0, colliderData1);
	}

	//���ʂƋ��̏ꍇ
	if (colliderData0.type == "Plane" && colliderData1.type == "Sphere")
	{
		return CheckPlaneSphere(colliderData0, colliderData1);
	}
	else if (colliderData0.type == "Sphere" && colliderData1.type == "Plane")
	{
		return CheckPlaneSphere(colliderData1, colliderData0);
	}

	//���ʂƃ{�b�N�X�̏ꍇ
	if (colliderData0.type == "Plane" && colliderData1.type == "Box")
	{
		return CheckPlaneBox(colliderData0, colliderData1);
	}
	else if (colliderData0.type == "Box" && colliderData1.type == "Plane")
	{
		return CheckPlaneBox(colliderData1, colliderData0);
	}

	return false;
}

bool ColliderManager::CheckSphereSphere(JSONLoader::ColliderData colliderSphere0, JSONLoader::ColliderData colliderSphere1)
{
	//���S���m�̋��������߂�
	XMFLOAT3 distanceCenter0 = colliderSphere0.center - colliderSphere1.center;
	float distanceCenter1 = sqrt(pow(distanceCenter0.x, 2) + pow(distanceCenter0.y, 2) + pow(distanceCenter0.z, 2));

	//��̃R���C�_�[�̔��a�̍��v
	float r = colliderSphere0.scale.x + colliderSphere1.scale.x;

	//���S�Ƃ̋��������a�̍��v���傫����Γ������Ă��Ȃ�
	if (r < distanceCenter1) return false;

	/*ChangeHitColor(colliderSphere0);*/
	ChangeHitColor(colliderSphere1);
	return true;
}

bool ColliderManager::CheckPlaneSphere(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderSphere)
{
	//���̒��S
	XMVECTOR sphereCenter = { colliderSphere.center.x, colliderSphere.center.y, colliderSphere.center.z, 1 };
	//���ʂ̖@���x�N�g��
	XMVECTOR planeNormal = { 0.0f,1.0f,0.0f,0.0f };
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR distV = {100,100,100,100};
	if (colliderPlane.center.x + colliderPlane.scale.x > colliderSphere.center.x - colliderSphere.scale.x &&
		colliderPlane.center.x - colliderPlane.scale.x < colliderSphere.center.x + colliderSphere.scale.x)
	{
		distV = XMVector3Dot(sphereCenter, planeNormal);
	}
	//���ʂ̒��S�Ƃ̋���
	float planeDistance = sqrt(pow(colliderPlane.center.x, 2) + 
		pow(colliderPlane.center.y, 2) + pow(colliderPlane.center.z, 2));
	//���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV.m128_f32[0] - planeDistance;

	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > colliderSphere.scale.x)return false;

	//�^����_���v�Z
	/*if(inter)*/

	ChangeHitColor(colliderPlane);

	return true;
}

bool ColliderManager::CheckPlaneBox(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderBox)
{
	//���̒��S
	XMVECTOR sphereCenter = { colliderBox.center.x, colliderBox.center.y, colliderBox.center.z, 1 };
	//���ʂ̖@���x�N�g��
	XMVECTOR planeNormal = { 0.0f,1.0f,0.0f,0.0f };
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR distV = { 100,100,100,100 };
	if (colliderPlane.center.x + colliderPlane.scale.x > colliderBox.center.x - colliderBox.scale.x &&
		colliderPlane.center.x - colliderPlane.scale.x < colliderBox.center.x + colliderBox.scale.x)
	{
		distV = XMVector3Dot(sphereCenter, planeNormal);
	}
	//���ʂ̒��S�Ƃ̋���
	float planeDistance = sqrt(pow(colliderPlane.center.x, 2) +
		pow(colliderPlane.center.y, 2) + pow(colliderPlane.center.z, 2));
	//���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV.m128_f32[0] - planeDistance;

	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > colliderBox.scale.y)return false;

	//�^����_���v�Z
	/*if(inter)*/

	ChangeHitColor(colliderPlane);

	return true;
}

ColliderManager::OBB ColliderManager::GetObbFromColliderData(JSONLoader::ColliderData colliderData)
{
	OBB o;

	//���S�_�����߂�
	o.c = colliderData.center;

	//�e���W���ɉ����������̔��������߂�
	o.e = colliderData.scale;

	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(colliderData.rotation.z);
	matRot *= XMMatrixRotationX(colliderData.rotation.x);
	matRot *= XMMatrixRotationY(colliderData.rotation.y);
	//�e���̌X���̕����x�N�g�������߂�
	o.u[0] = { matRot.r[0].m128_f32[0],matRot.r[1].m128_f32[0] ,matRot.r[2].m128_f32[0] };
	o.u[1] = { matRot.r[0].m128_f32[1],matRot.r[1].m128_f32[1] ,matRot.r[2].m128_f32[1] };
	o.u[2] = { matRot.r[0].m128_f32[2],matRot.r[1].m128_f32[2] ,matRot.r[2].m128_f32[2] };

	return o;
}

void ColliderManager::ChangeHitColor(JSONLoader::ColliderData colliderData)
{
	//�������Ă��镽�ʂ̐F��ύX
	for (std::unique_ptr<Collider>& colliders : collider)
	{
		if (colliders->colliderData.objectName == colliderData.objectName)
		{
			if (colliderData.type == "Plane")colliders->colliderPlaneObject->SetColor(isHitColor);
			if (colliderData.type == "Sphere")colliders->colliderSphereObject->SetColor(isHitColor);
			if (colliderData.type == "Box")colliders->colliderCubeObject->SetColor(isHitColor);
		}
	}
}
