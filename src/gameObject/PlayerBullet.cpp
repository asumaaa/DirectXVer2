#include "PlayerBullet.h"
#include "mathOriginal.h"
#include "ColliderManager.h"

Camera* PlayerBullet::camera = nullptr;
Input* PlayerBullet::input = nullptr;
DXInput* PlayerBullet::dxInput = nullptr;
FbxModel* PlayerBullet::model = nullptr;

void PlayerBullet::Initialize()
{
}

void PlayerBullet::Update()
{
	//弾を消す処理
	DeleteBullet();

	//ショットフラグが立ったら弾生成
	if (shotFlag)
	{
		CreateBullet();
	}

	//動く
	Move();

	//タイマー加算
	for (int i = 0; i < object.size(); i++)
	{
		timer[i] += 1.0f;
	}

	//更新
	int i = 0;
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->SetPosition(position[i]);
		objects->SetRotation(rotation[i]);
		objects->SetScale(scale[i]);
		objects->Update();
		i++;
	}

	//ショットフラグを偽に
	shotFlag = false;
}

void PlayerBullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	/*for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->Draw(cmdList);
	}*/
}

void PlayerBullet::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	/*for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->DrawLightView(cmdList);
	}*/
}

void PlayerBullet::Move()
{
	if (object.size() != 0)
	{
		for (int i = 0; i < object.size(); i++)
		{
			//進行ベクトルを加算
			position[i] = position[i] + (velocity[i] * posSpeed);
		}
	}
}

void PlayerBullet::CreateBullet()
{
	//オブジェクト生成
	std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
	newObject->Initialize();
	newObject->SetModel(model);

	//オブジェクトごとに名前をつける
	std::string objectName = "playerBullet" + std::to_string(number);

	//オブジェクトデータセット
	JSONLoader::ObjectData objectData;
	objectData.fileName = "playerBullet";
	objectData.objectName = objectName;
	objectData.position = position.back();
	objectData.rotation = rotation.back();
	objectData.scale = scale.back();
	newObject->SetObjectData(objectData);

	//コライダーデータセット
	JSONLoader::ColliderData colliderData;
	colliderData.type = "Sphere";	//判定を球体で取るため
	colliderData.objectName = objectName;
	colliderData.scale = scale.back();
	colliderData.rotation = rotation.back();
	colliderData.center = position.back();
	newObject->SetColliderData(colliderData);

	//コライダーマネージャーにセット
	ColliderManager::SetCollider(colliderData);

	//オブジェクトに代入
	object.emplace_back(std::move(newObject));

	number++;
}

void PlayerBullet::DeleteBullet()
{
	//削除
	for (int i = 0; i < object.size(); i++)
	{
		//一定時間経ったら
		if (timer[i] == destoryTime)
		{
			position.erase(position.begin() + i);
			rotation.erase(rotation.begin() + i);
			scale.erase(scale.begin() + i);
			velocity.erase(velocity.begin() + i);
			hitFlag.erase(hitFlag.begin() + i);
			timer.erase(timer.begin() + i);
			object.erase(std::next(object.begin(), i));

			continue;
		}

		//ヒットフラグが立ったら
		if (hitFlag[i] == true)
		{
			position.erase(position.begin() + i);
			rotation.erase(rotation.begin() + i);
			scale.erase(scale.begin() + i);
			velocity.erase(velocity.begin() + i);
			timer.erase(timer.begin() + i);
			hitFlag.erase(hitFlag.begin() + i);
			object.erase(std::next(object.begin(), i));
		}
	}
}

void PlayerBullet::SetSRV(ID3D12DescriptorHeap* SRV)
{
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->SetSRV(SRV);
	}
}

void PlayerBullet::SetBullet(XMFLOAT3 position, XMFLOAT3 velocity)
{
	PlayerBullet::position.emplace_back(position);
	PlayerBullet::rotation.emplace_back(baseRotation);
	PlayerBullet::scale.emplace_back(baseScale);
	PlayerBullet::velocity.emplace_back(velocity);
	PlayerBullet::timer.emplace_back(0.0f);
	PlayerBullet::hitFlag.emplace_back(false);
}

JSONLoader::ColliderData PlayerBullet::GetColliderData(int num)
{
	std::list<std::unique_ptr<FbxObject3D>>::iterator itr;
	itr = object.begin();
	if (num != 0)
	{
		for (int i = 0; i < num; i++)
		{
			itr++;
		}
	}

	return itr->get()->GetColliderData();
}
