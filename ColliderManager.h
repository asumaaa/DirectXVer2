#pragma once
#include "ColliderCubeModel.h"
#include "ColliderCubeObject.h"
#include "ColliderSphereModel.h"
#include "ColliderSphereObject.h"
#include "JSONLoader.h"

class ColliderManager
{
private:	//サブクラス
	struct Collider 
	{
		JSONLoader::ColliderData colliderData;
		ColliderCubeObject* colliderCubeObject = nullptr;
		ColliderSphereObject* colliderSphereObject = nullptr;
		//描画フラグ
		bool drawFlag = false;
	};

public:	//静的メンバ関数
	void SetCollider(JSONLoader::ColliderData colliderData);
	void SetColliderCubeModel(ColliderCubeModel* colliderModel) { ColliderManager::colliderCubeModel = colliderModel; }
	void SetColliderSphereModel(ColliderSphereModel* colliderModel) { ColliderManager::colliderSphereModel = colliderModel; };

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

private:	//メンバ関数
	//モデル
	ColliderCubeModel*colliderCubeModel;
	ColliderSphereModel*colliderSphereModel;
	//オブジェクト
	/*std::list<std::unique_ptr<ColliderCubeObject>>colliderCubeObject;
	std::list<std::unique_ptr<ColliderSphereObject>>colliderSphereObject;*/
	//コライダー
	std::list<std::unique_ptr<Collider>>collider;

	//当たっていないときの色
	XMFLOAT4 noHitColor = { 0,0,1,1 };
};

