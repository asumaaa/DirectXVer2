/**
 * @file EnemyBullet.cpp
 * @brief ゲームオブジェクト 敵の弾
 * @author Asuma Syota
 * @date 2023/12
 */

#include "EnemyBullet.h"
#include "mathOriginal.h"
#include "ColliderManager.h"

void EnemyBullet::Initialize()
{
}

void EnemyBullet::Update()
{
	UpdateCollider();
}

void EnemyBullet::UpdateCollider()
{
	for (int i = 0; i < bullet.size(); i++)
	{	
		//各データを代入
		bullet[i].colliderData.center = bullet[i].position;
		bullet[i].colliderData.scale = bullet[i].scale;
		bullet[i].colliderData.rotation = bullet[i].rotation;
	}
}

void EnemyBullet::Draw()
{
}

void EnemyBullet::Move()
{
}

void EnemyBullet::CreateBullet()
{
}

void EnemyBullet::DeleteBullet()
{
}

void EnemyBullet::SetBullet(XMFLOAT3 position, XMFLOAT3 velocity)
{
	//引数から初期値設定
	Bullet b;
	b.position = position;
	b.rotation = baseRotation;
	b.scale = baseScale;
	b.velocity = velocity;
	b.timer = 0.0f;
	b.hitFlag = false;

	//コライダーの設定
	//コライダーごとに名前をつける
	std::string objectName = "enemyBullet" + std::to_string(number);
	//コライダーデータの生成
	JSONLoader::ColliderData colliderData;
	colliderData.type = "Sphere";	//判定を球体で取るため
	colliderData.objectName = objectName;
	colliderData.scale = b.scale;
	colliderData.rotation = b.rotation;
	colliderData.center = b.position;
	b.colliderData = colliderData;

	//コライダーマネージャーにセット
	ColliderManager::SetCollider(colliderData);

	//設定した値を挿入
	bullet.emplace_back(b);

	//弾につける番号を増やす
	number++;
}
