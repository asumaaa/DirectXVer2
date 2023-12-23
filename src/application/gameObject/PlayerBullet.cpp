/**
 * @file PlayerBullet.cpp
 * @brief ゲームオブジェクト 自機の弾
 * @author Asuma Syota
 * @date 2023/12
 */

#include "PlayerBullet.h"
#include "mathOriginal.h"
#include "ColliderManager.h"
#include "imgui.h"

Camera* PlayerBullet::camera = nullptr;

void PlayerBullet::Initialize()
{
	PlayerBulletParticle* newParticle = new PlayerBulletParticle();
	newParticle->CreateBuffers();
	newParticle->SetTextureNum(6);
	particle.reset(newParticle);
}

void PlayerBullet::Update()
{
	for (int i = 0; i < bullet.size(); i++)
	{
		bullet[i].timer += 1.0f;
	}

	//弾を消す処理
	DeleteBullet();

	//動き
	Move();

	//コライダー更新
	UpdateCollider();

	//パーティクル更新
	UpdateParticle();
}

void PlayerBullet::UpdateCollider()
{
	for (int i = 0; i < bullet.size(); i++)
	{	
		//各データを代入
		bullet[i].colliderData.center = bullet[i].position1;
		bullet[i].colliderData.scale = bullet[i].scale;
	}
}

void PlayerBullet::UpdateParticle()
{
	for (int i = 0; i < bullet.size(); i++)
	{
		//弾の座標にパーティクル追加
		particle->Add(bullet[i].position1);
	}

	//更新
	particle->Update();
}

void PlayerBullet::Draw()
{
	////ImGui
	//ImGui::Begin("EnemyBullet");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 150));
	//ImGui::InputFloat3("EnemyBulletPos", bulletPos);
	//ImGui::End();
}

void PlayerBullet::DrawParticle(ID3D12GraphicsCommandList* cmdList)
{
	//更新
	particle->Draw(cmdList);
}

void PlayerBullet::Move()
{
	for (int i = 0; i < bullet.size(); i++)
	{
		//弾の座標加算
		if (bullet[i].timer < bullet[i].destoryTime)
		{
			bullet[i].position1 = bullet[i].position1 + bullet[i].velocity;
		}
	}
		//弾発射後
	//	else if(bullet[i].timer < bullet[i].destoryTime1 + bullet[i].destoryTime2)
	//	{
	//		//最初にベクトル取得
	//		if (bullet[i].timer < bullet[i].destoryTime1 + 1)
	//		{
	//			bullet[i].velocity = enemyPos - bullet[i].position1;
	//			bullet[i].velocity = normalize(bullet[i].velocity);

	//			//最初の弾の座標保存
	//			bullet[i].position2 = bullet[i].position1;
	//			//プレイヤーの座標保存
	//			bullet[i].position3 = enemyPos;
	//		}

	//		//ベジエ曲線用中間地点を計算
	//		XMFLOAT3 halfPos = (bullet[i].position3 - bullet[i].position2) / 2.0f;
	//		halfPos.y += 100.0f;
	//		//ベジエ曲線を計算
	//		float t = (bullet[i].timer - bullet[i].destoryTime1) / bullet[i].destoryTime2;
	//		XMFLOAT3 a = lerp(bullet[i].position2, halfPos,easeInCubic(t)* speed);
	//		XMFLOAT3 b = lerp(halfPos, bullet[i].position3, easeInCubic(t) * speed);
	//		bullet[i].position1 = lerp(a, b, easeInCubic(t) * speed);

	//		//プレイヤーから弾のベクトル取得
	//		//XMFLOAT3 velo = playerPos - bullet[i].position;
	//		//velo = normalize(velo);
	//		////弾の座標に加算
	//		//bullet[i].position = bullet[i].position + velo * speed;
	//	}
	//}
}

void PlayerBullet::CreateBullet()
{
}

void PlayerBullet::DeleteBullet()
{
	//削除
	for (int i = 0; i < bullet.size(); i++)
	{
		//一定時間経ったら
		if (bullet[i].timer == bullet[i].destoryTime)
		{
			//弾削除
			bullet.erase(bullet.begin() + i);

			continue;
		}

		//ヒットフラグが立ったら
		if (bullet[i].hitFlag == true)
		{
			//弾削除
			bullet.erase(bullet.begin() + i);
		}
	}
}

void PlayerBullet::SetBullet(XMFLOAT3 position,XMFLOAT3 velocity,float timer,float destoryTime)
{
	//引数から初期値設定
	Bullet b;
	b.position1 = position;
	b.velocity = velocity;
	b.timer = timer;
	b.destoryTime = destoryTime;
	b.hitFlag = false;

	//コライダーの設定
	//コライダーごとに名前をつける
	std::string objectName = "enemyBullet" + std::to_string(number);
	//コライダーデータの生成
	JSONLoader::ColliderData colliderData;
	colliderData.type = "Sphere";	//判定を球体で取るため
	colliderData.objectName = objectName;
	colliderData.scale = b.scale;
	colliderData.rotation = XMFLOAT3(0.0f,0.0f,0.0f);
	colliderData.center = b.position1;
	b.colliderData = colliderData;

	//コライダーマネージャーにセット
	ColliderManager::SetCollider(colliderData);

	//設定した値を挿入
	bullet.emplace_back(b);

	//弾につける番号を増やす
	number++;
}
