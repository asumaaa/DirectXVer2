#pragma once
#include "ColliderCubeObject.h"
#include "ColliderSphereObject.h"
#include "ColliderPlaneObject.h"
#include "JSONLoader.h"

class ColliderManager
{
public:	//サブクラス
	//コライダー描画用
	struct Collider 
	{
		JSONLoader::ColliderData colliderData;
		ColliderCubeObject* colliderCubeObject = nullptr;
		ColliderSphereObject* colliderSphereObject = nullptr;
		ColliderPlaneObject* colliderPlaneObject = nullptr;
		//描画フラグ
		bool drawFlag = false;
	};

	//OBB 判定計算用
	struct OBB
	{
		XMFLOAT3 c;	//中心点
		XMFLOAT3 u[3];	//XYZの各座標軸の傾きを表すベクトル
		XMFLOAT3 e;	//OBBの各座標軸に沿った長さの半分
	};

public:	//静的メンバ関数
	void SetCollider(JSONLoader::ColliderData colliderData);
	void SetColliderCubeModel(ColliderCubeModel* colliderModel) { ColliderManager::colliderCubeModel = colliderModel; }
	void SetColliderSphereModel(ColliderSphereModel* colliderModel) { ColliderManager::colliderSphereModel = colliderModel; };
	void SetColliderPlaneModel(ColliderPlaneModel* colliderModel) { ColliderManager::colliderPlaneModel = colliderModel; };

public:	//メンバ関数
	//初期化
	void Initialize();
	//更新
	void PreUpdate();
	void PostUpdate();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//判定
	bool CheckCollider(JSONLoader::ColliderData colliderData0, JSONLoader::ColliderData colliderData1);
	//球体と球体
	bool CheckSphereSphere(JSONLoader::ColliderData colliderSphere0, JSONLoader::ColliderData colliderSphere1);
	//平面と球体
	bool CheckPlaneSphere(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderSphere);
	//平面とボックス
	bool CheckPlaneBox(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderBox);

	//ColliderDataクラスからOBBを返す関数	Box,平面のみ対応
	OBB GetObbFromColliderData(JSONLoader::ColliderData colliderData);
	//色を変える関数
	void ChangeHitColor(JSONLoader::ColliderData colliderData);

private:	//メンバ関数
	//モデル
	ColliderCubeModel*colliderCubeModel;
	ColliderSphereModel* colliderSphereModel;
	ColliderPlaneModel*colliderPlaneModel;
	//コライダー
	std::list<std::unique_ptr<Collider>>collider;

	//当たっていないときの色
	XMFLOAT4 noHitColor = { 0,0,1,1 };
	XMFLOAT4 isHitColor = { 0,1,0,1 };
};

