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
	static void SetCollider(JSONLoader::ColliderData colliderData);
	static void SetColliderCubeModel(ColliderCubeModel* colliderModel) { ColliderManager::colliderCubeModel = colliderModel; }
	static void SetColliderSphereModel(ColliderSphereModel* colliderModel) { ColliderManager::colliderSphereModel = colliderModel; };
	static void SetColliderPlaneModel(ColliderPlaneModel* colliderModel) { ColliderManager::colliderPlaneModel = colliderModel; };

public:	//メンバ関数
	//初期化
	static void Initialize();
	//更新
	static void PreUpdate();
	static void PostUpdate();
	//描画
	static void Draw(ID3D12GraphicsCommandList* cmdList);

	//判定
	static bool CheckCollider(JSONLoader::ColliderData colliderData0, JSONLoader::ColliderData colliderData1);
	//球体と球体
	static bool CheckSphereSphere(JSONLoader::ColliderData colliderSphere0, JSONLoader::ColliderData colliderSphere1);
	//平面と球体
	static bool CheckPlaneSphere(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderSphere);
	//平面とボックス
	static bool CheckPlaneBox(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderBox);

	//ColliderDataクラスからOBBを返す関数	Box,平面のみ対応
	OBB GetObbFromColliderData(JSONLoader::ColliderData colliderData);
	//色を変える関数
	static void ChangeHitColor(JSONLoader::ColliderData colliderData);

public:	//静的メンバ変数
	//コライダー
	static std::list<std::unique_ptr<Collider>>collider;
	//モデル
	static ColliderCubeModel* colliderCubeModel;
	static ColliderSphereModel* colliderSphereModel;
	static ColliderPlaneModel* colliderPlaneModel;

	//当たっていないときの色
	static XMFLOAT4 noHitColor;
	static XMFLOAT4 isHitColor;
};

