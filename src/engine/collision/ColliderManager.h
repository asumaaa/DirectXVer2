/**
 * @file ColliderManager.h
 * @brief 判定の一括管理を行う
 * @author Asuma Syota
 * @date 2023/4
 */

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

	/// <summary>
	///コライダーデータセット
	/// </summary>
	static void SetCollider(JSONLoader::ColliderData colliderData);

	/// <summary>
	///モデルセット
	/// </summary>
	static void SetColliderCubeModel(ColliderCubeModel* colliderModel) { ColliderManager::colliderCubeModel = colliderModel; }

	/// <summary>
	///モデルセット
	/// </summary>
	static void SetColliderSphereModel(ColliderSphereModel* colliderModel) { ColliderManager::colliderSphereModel = colliderModel; };

	/// <summary>
	///モデルセット
	/// </summary>
	static void SetColliderPlaneModel(ColliderPlaneModel* colliderModel) { ColliderManager::colliderPlaneModel = colliderModel; };

public:	//メンバ関数

	/// <summary>
	///初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	///更新 前
	/// </summary>
	static void PreUpdate();

	/// <summary>
	///更新 後
	/// </summary>
	static void PostUpdate();

	/// <summary>
	///描画
	/// </summary>
	static void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	///判定
	/// </summary>
	static bool CheckCollider(JSONLoader::ColliderData colliderData0, JSONLoader::ColliderData colliderData1);

	/// <summary>
	///判定 球体と球体
	/// </summary>
	static bool CheckSphereSphere(JSONLoader::ColliderData colliderSphere0, JSONLoader::ColliderData colliderSphere1);

	/// <summary>
	///判定 平面と球体
	/// </summary>
	static bool CheckPlaneSphere(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderSphere);

	/// <summary>
	///判定 平面とボックス
	/// </summary>
	static bool CheckPlaneBox(JSONLoader::ColliderData colliderPlane, JSONLoader::ColliderData colliderBox);

	/// <summary>
	///ColliderDataクラスからOBBを返す関数	Box,平面のみ対応
	/// </summary>
	OBB GetObbFromColliderData(JSONLoader::ColliderData colliderData);

	/// <summary>
	///当たったら色を変える変数
	/// </summary>
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
