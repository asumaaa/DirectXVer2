#include "Plane.h"

Camera* Plane::camera = nullptr;
Input* Plane::input = nullptr;
DXInput* Plane::dxInput = nullptr;

void Plane::Initialize()
{
}

void Plane::Update()
{
	//“®‚­
	Move();

	/*object->SetPosition(position);
	object->SetRotation(rotation);
	object->SetScale(scale);*/

	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->Update();
	}
}

void Plane::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->Draw(cmdList);
	}
}

void Plane::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->DrawLightView(cmdList);
	}
}

void Plane::Move()
{
}

void Plane::SetObject(FbxObject3D* object)
{
	/*std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
	newObject.reset(object);
	newObject->SetPosition(object->GetPosition());
	newObject->SetRotation(object->GetRotation());
	newObject->SetScale(object->GetScale());*/
	Plane::object.emplace_back(object);
}

void Plane::SetSRV(ID3D12DescriptorHeap* SRV)
{
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->SetSRV(SRV);
	}
}
