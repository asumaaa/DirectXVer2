#include "ColliderManager.h"
#include "math.h"
#include "mathOriginal.h"

using namespace DirectX;
#define PI 3.1415

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
	//球体の場合
	if (colliderData.type == "Sphere")
	{
		ColliderSphereObject* newCollider = new ColliderSphereObject();
		newCollider->Initialize();
		newColliderData->colliderSphereObject = newCollider;
	}
	//ボックスの場合
	if (colliderData.type == "Box")
	{
		ColliderCubeObject* newCollider = new ColliderCubeObject();
		newCollider->Initialize();
		newColliderData->colliderCubeObject = newCollider;
	}
	//平面の場合
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
		if (colliders->colliderData.type == "Plane")
		{
			colliders->colliderPlaneObject->SetColor(noHitColor);
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

	//平面と球の場合
	if (colliderData0.type == "Plane" && colliderData1.type == "Sphere")
	{
		return CheckPlaneSphere(colliderData0, colliderData1);
	}
	else if (colliderData0.type == "Sphere" && colliderData1.type == "Plane")
	{
		return CheckPlaneSphere(colliderData1, colliderData0);
	}

	//平面とボックスの場合
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

bool ColliderManager::CheckPlaneSphere(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderSphere)
{
	//球の中心
	XMVECTOR sphereCenter = { colliderSphere.center.x, colliderSphere.center.y, colliderSphere.center.z, 1 };
	//平面の法線ベクトル
	XMVECTOR planeNormal = { 0.0f,1.0f,0.0f,0.0f };
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = {100,100,100,100};
	if (colliderPlane.center.x + colliderPlane.scale.x > colliderSphere.center.x - colliderSphere.scale.x &&
		colliderPlane.center.x - colliderPlane.scale.x < colliderSphere.center.x + colliderSphere.scale.x)
	{
		distV = XMVector3Dot(sphereCenter, planeNormal);
	}
	//平面の中心との距離
	float planeDistance = sqrt(pow(colliderPlane.center.x, 2) + 
		pow(colliderPlane.center.y, 2) + pow(colliderPlane.center.z, 2));
	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - planeDistance;

	//距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > colliderSphere.scale.x)return false;

	//疑似交点を計算
	/*if(inter)*/

	ChangeHitColor(colliderPlane);

	return true;
}

bool ColliderManager::CheckPlaneBox(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderBox)
{
	OBB obb1 = GetObbFromColliderData(colliderPlane);
	OBB obb2 = GetObbFromColliderData(colliderBox);

	// 各方向ベクトルの確保
	XMFLOAT3 NAe1 = obb1.u[0], Ae1 = NAe1 * length(obb1.u[0]);
	XMFLOAT3 NAe2 = obb1.u[1], Ae2 = NAe2 * length(obb1.u[1]);
	XMFLOAT3 NAe3 = obb1.u[2], Ae3 = NAe3 * length(obb1.u[2]);
	XMFLOAT3 NBe1 = obb2.u[0], Be1 = NBe1 * length(obb2.u[0]);
	XMFLOAT3 NBe2 = obb2.u[1], Be2 = NBe2 * length(obb2.u[1]);
	XMFLOAT3 NBe3 = obb2.u[2], Be3 = NBe3 * length(obb2.u[2]);
	XMFLOAT3 Interval = obb1.c - obb2.c;

	// 分離軸 : Ae1
	FLOAT rA = length(Ae1);
	FLOAT rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	FLOAT L = fabs(dot(Interval, NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	// 分離軸 : Ae2
	/*rA = length(Ae2);
	rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabs(dot(Interval, NAe2));
	if (L > rA + rB)
		return false;*/

	//// 分離軸 : Ae3
	//rA = length(Ae3);
	//rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	//L = fabs(dot(Interval, NAe3));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : Be1
	//rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	//rB = length(Be1);
	//L = fabs(dot(Interval, NBe1));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : Be2
	//rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	//rB = length(Be2);
	//L = fabs(dot(Interval, NBe2));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : Be3
	//rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	//rB = length(Be3);
	//L = fabs(dot(Interval, NBe3));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C11
	//XMFLOAT3 Cross;
	//Cross = cross(NAe1, NBe1);
	//rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	//rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	//L = fabs(dot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C12
	//Cross = cross(NAe1, NBe2);
	//rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	//rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	//L = fabs(dot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C13
	//Cross = cross(NAe1, NBe3);
	//rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	//rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	//L = fabs(dot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C21
	//Cross = cross(NAe2, NBe1);
	//rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	//rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	//L = fabs(dot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C22
	//Cross = cross(NAe2, NBe2);
	//rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	//rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	//L = fabs(dot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C23
	//Cross = cross(NAe2, NBe3);
	//rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	//rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	//L = fabs(dot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C31
	//Cross = cross(NAe3, NBe1);
	//rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	//rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	//L = fabs(dot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C32
	//Cross = cross(NAe3, NBe2);
	//rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	//rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	//L = fabs(dot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C33
	//Cross = cross(NAe3, NBe3);
	//rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	//rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	//L = fabs(dot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	ChangeHitColor(colliderPlane);
	return true;
}

ColliderManager::OBB ColliderManager::GetObbFromColliderData(JSONLoader::ColliderData colliderData)
{
	OBB o;

	//中心点を求める
	o.c = colliderData.center;

	//各座標軸に沿った長さの半分を求める
	o.e = colliderData.scale;

	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(colliderData.rotation.z);
	matRot *= XMMatrixRotationX(colliderData.rotation.x);
	matRot *= XMMatrixRotationY(colliderData.rotation.y);
	//各軸の傾きの方向ベクトルを求める
	o.u[0] = { matRot.r[0].m128_f32[0],matRot.r[1].m128_f32[0] ,matRot.r[2].m128_f32[0] };
	o.u[1] = { matRot.r[0].m128_f32[1],matRot.r[1].m128_f32[1] ,matRot.r[2].m128_f32[1] };
	o.u[2] = { matRot.r[0].m128_f32[2],matRot.r[1].m128_f32[2] ,matRot.r[2].m128_f32[2] };

	return o;
}

void ColliderManager::ChangeHitColor(JSONLoader::ColliderData colliderData)
{
	//当たっている平面の色を変更
	for (std::unique_ptr<Collider>& colliders : collider)
	{
		if (colliders->colliderData.objectName == colliderData.objectName)
		{
			colliders->colliderPlaneObject->SetColor(isHitColor);
		}
	}
}
