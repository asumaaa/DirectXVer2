#include "ColliderManager.h"

void ColliderManager::SetCollider(JSONLoader::ColliderData colliderData)
{
	//コライダーセット
	std::unique_ptr<Collider>newColliderData = std::make_unique<Collider>();
	newColliderData->colliderData.type = colliderData.type;
	newColliderData->colliderData.objectName = colliderData.objectName;
	newColliderData->colliderData.scale = colliderData.scale;
	newColliderData->colliderData.rotation = colliderData.rotation;
	newColliderData->colliderData.center = colliderData.center;

	//オブジェクト生成
	if (colliderData.type == "Sphere")
	{
		ColliderSphereObject* newCollider = new ColliderSphereObject();
		newCollider->Initialize();
		newColliderData->colliderSphereObject = newCollider;
	}

	if (colliderData.type == "Box")
	{
		ColliderCubeObject* newCollider = new ColliderCubeObject();
		newCollider->Initialize();
		newColliderData->colliderCubeObject = newCollider;
	}
	collider.emplace_back(std::move(newColliderData));
}

void ColliderManager::Initialize()
{
}

void ColliderManager::PreUpdate()
{
	//オブジェクトの更新
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
	}
}

void ColliderManager::PostUpdate()
{
	//オブジェクトの更新
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
		}
	}
}

bool ColliderManager::CheckCollider(JSONLoader::ColliderData colliderData0, JSONLoader::ColliderData colliderData1)
{

	for (std::unique_ptr<Collider>& colliders : collider)
	{
		//描画フラグを立てる 位置更新
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
	return false;
}
