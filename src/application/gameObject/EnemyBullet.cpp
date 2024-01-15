/**
 * @file EnemyBullet.cpp
 * @brief ゲームオブジェクト 敵の弾
 * @author Asuma Syota
 * @date 2023/12
 */

#include "EnemyBullet.h"
#include "mathOriginal.h"
#include "ColliderManager.h"
#include "imgui.h"

Camera* EnemyBullet::camera = nullptr;

void EnemyBullet::Initialize()
{
	EnemyBulletParticle* newParticle = new EnemyBulletParticle();
	newParticle->CreateBuffers();
	newParticle->SetTextureNum(4);
	particle.reset(newParticle);
}

void EnemyBullet::Update()
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

void EnemyBullet::UpdateCollider()
{
	for (int i = 0; i < bullet.size(); i++)
	{	
		//各データを代入
		bullet[i].colliderData.center = bullet[i].position1;
		bullet[i].colliderData.scale = bullet[i].scale;
		bullet[i].colliderData.rotation = bullet[i].rotation;
	}
}

void EnemyBullet::UpdateParticle()
{
	for (int i = 0; i < bullet.size(); i++)
	{
		//弾の座標にパーティクル追加
		particle->Add(bullet[i].position1);
	}

	//更新
	particle->Update();
}

void EnemyBullet::Draw()
{
	////ImGui
	//ImGui::Begin("EnemyBullet");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 150));
	//ImGui::InputFloat3("EnemyBulletPos", bulletPos);
	//ImGui::End();
}

void EnemyBullet::DrawParticle(ID3D12GraphicsCommandList* cmdList)
{
	//更新
	particle->Draw(cmdList);
}

void EnemyBullet::Move()
{
	for (int i = 0; i < bullet.size(); i++)
	{
		//溜めモーション中
		if (bullet[i].timer < bullet[i].destoryTime1)
		{
			//if (bullet[i].timer == 0)
			//{
			//	//最初の座標保存
			//	bullet[i].position2 = bullet
			//}
			/*bullet[i].position1 = XMFLOAT3(-20, 40, 50);*/
		}
		//弾発射後
		else if(bullet[i].timer < bullet[i].destoryTime1 + bullet[i].destoryTime2)
		{
			//最初にベクトル取得
			if (bullet[i].timer < bullet[i].destoryTime1 + 1)
			{
				bullet[i].velocity = playerPos - bullet[i].position1;
				bullet[i].velocity = normalize(bullet[i].velocity);

				//最初の弾の座標保存
				bullet[i].position2 = bullet[i].position1;
				//プレイヤーの座標保存
				bullet[i].position3 = playerPos;
			}

			//ベジエ曲線用中間地点を計算
			XMFLOAT3 halfPos = (bullet[i].position3 - bullet[i].position2) / 2.0f;
			halfPos.y += 100.0f;
			//ベジエ曲線を計算
			float t = (bullet[i].timer - bullet[i].destoryTime1) / bullet[i].destoryTime2;
			XMFLOAT3 a = lerp(bullet[i].position2, halfPos,easeInCubic(t)* speed);
			XMFLOAT3 b = lerp(halfPos, bullet[i].position3, easeInCubic(t) * speed);
			bullet[i].position1 = lerp(a, b, easeInCubic(t) * speed);

			//プレイヤーから弾のベクトル取得
			//XMFLOAT3 velo = playerPos - bullet[i].position;
			//velo = normalize(velo);
			////弾の座標に加算
			//bullet[i].position = bullet[i].position + velo * speed;
		}
	}
}

void EnemyBullet::CreateBullet()
{
}

void EnemyBullet::DeleteBullet()
{
	//削除
	for (int i = 0; i < bullet.size(); i++)
	{
		//一定時間経ったら
		if (bullet[i].timer == bullet[i].destoryTime1 + bullet[i].destoryTime2)
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

void EnemyBullet::SetBullet(XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 lastScale, float timer, float destoryTime1, float destoryTime2)
{
	//引数から初期値設定
	Bullet b;
	b.position1 = position;
	b.position2 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	b.position3 = XMFLOAT3(0.0f,0.0f,0.0f);
	b.rotation = baseRotation;
	b.scale = scale;
	b.lastScale = lastScale;
	b.velocity = XMFLOAT3(0.0f,0.0f,1.0f);
	b.timer = timer;
	b.destoryTime1 = destoryTime1;
	b.destoryTime2 = destoryTime2;
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
	colliderData.center = b.position1;
	b.colliderData = colliderData;

	//コライダーマネージャーにセット
	ColliderManager::SetCollider(colliderData);

	//設定した値を挿入
	bullet.emplace_back(b);

	//弾につける番号を増やす
	number++;
}
