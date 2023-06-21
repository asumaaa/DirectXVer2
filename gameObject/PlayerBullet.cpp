#include "PlayerBullet.h"
#include "mathOriginal.h"

Camera* PlayerBullet::camera = nullptr;
Input* PlayerBullet::input = nullptr;
DXInput* PlayerBullet::dxInput = nullptr;
FbxModel* PlayerBullet::model = nullptr;

void PlayerBullet::Initialize()
{
}

void PlayerBullet::Update()
{
	//ショットフラグが立ったら弾生成
	if (shotFlag)
	{
		CreateBullet();
	}

	//動く
	Move();

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
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->Draw(cmdList);
	}
}

void PlayerBullet::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->DrawLightView(cmdList);
	}
}

void PlayerBullet::Move()
{
	for (int i = 0; i < object.size(); i++)
	{
		//進行ベクトルを加算
		position[i] = position[i] + velocity[i];
	}
}

void PlayerBullet::CreateBullet()
{
	std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
	newObject->Initialize();
	newObject->SetModel(model);
	/*newBullet->position = { 0.0f,0.0f,0.0f };
	newBullet->rotation = { 0.0f,0.0f,0.0f };
	newBullet->scale = { 0.0f,0.0f,0.0f };*/

	//オブジェクトデータセット
	JSONLoader::ObjectData objectData;
	objectData.fileName = "playerBullet";
	objectData.objectName = "playerBullet";
	objectData.position = position.back();
	objectData.rotation = rotation.back();
	objectData.scale = scale.back();
	newObject->SetObjectData(objectData);

	//コライダーデータセット
	JSONLoader::ColliderData colliderData;
	colliderData.type = "Sphere";	//判定を球体で取るため
	colliderData.objectName = "playerBullet";
	colliderData.scale = scale.back();
	colliderData.rotation = rotation.back();
	colliderData.center = position.back();
	newObject->SetColliderData(colliderData);

	object.emplace_back(std::move(newObject));
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
}
