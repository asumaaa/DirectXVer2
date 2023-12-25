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
	//タイマー更新
	for (std::forward_list<Bullet>::iterator it = bullet.begin(); it != bullet.end(); it++)
	{
		it->timer += 1.0f;
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
	for (std::forward_list<Bullet>::iterator it = bullet.begin(); it != bullet.end(); it++)
	{
		//各データを代入
		it->colliderData.center = it->position1;
		it->colliderData.scale = it->scale;
		it->colliderData.rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
}

void PlayerBullet::UpdateParticle()
{
	for (std::forward_list<Bullet>::iterator it = bullet.begin(); it != bullet.end(); it++)
	{
		//弾の座標にパーティクル追加
		particle->Add(it->position1);
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
	for (std::forward_list<Bullet>::iterator it = bullet.begin(); it != bullet.end(); it++)
	{
		it->position1 = it->position1 + it->velocity;
	}
}

void PlayerBullet::CreateBullet()
{
}

void PlayerBullet::DeleteBullet()
{
	//寿命が尽きた球を全削除
	bullet.remove_if([](Bullet& b)
		{
			return b.timer >= b.destoryTime;
		});

	//ヒットフラグが立った弾を全削除
	bullet.remove_if([](Bullet& b)
		{
			return b.hitFlag == true;
		});
}

void PlayerBullet::SetHitFlag(int num)
{
	//先頭の要素取得
	std::forward_list<Bullet>::iterator it = bullet.begin();
	//ヒットフラグを立てたい要素まで進める
	for (int i = 0; i < num; i++)
	{
		++it;
	}
	it->hitFlag = true;
}

JSONLoader::ColliderData PlayerBullet::GetColliderData(int num)
{
	//先頭の要素取得
	std::forward_list<Bullet>::iterator it = bullet.begin();
	//コライダーを取得したい要素まで進める
	for (int i = 0; i < num; i++)
	{
		++it;
	}
	return it->colliderData;
}

XMFLOAT3 PlayerBullet::GetPosition(int num)
{
	//先頭の要素取得
	std::forward_list<Bullet>::iterator it = bullet.begin();
	//座標を取得したい要素まで進める
	for (int i = 0; i < num; i++)
	{
		++it;
	}
	return it->position1;
}

void PlayerBullet::SetBullet(XMFLOAT3 position,XMFLOAT3 velocity,float timer,float destoryTime)
{
	//引数から初期値設定
	//リストに要素を追加
	bullet.emplace_front();
	//追加した要素の参照
	Bullet& b = bullet.front();
	//値のセット
	b.position1 = position;
	b.scale = XMFLOAT3(3.0f, 3.0f, 3.0f);
	b.velocity = velocity;
	b.timer = timer;
	b.destoryTime = destoryTime;
	b.hitFlag = false;

	//コライダーの設定
	//コライダーごとに名前をつける
	std::string objectName = "playerBullet" + std::to_string(number);
	//コライダーデータの生成
	JSONLoader::ColliderData colliderData;
	colliderData.type = "Sphere";	//判定を球体で取るため
	colliderData.objectName = objectName;
	colliderData.scale = XMFLOAT3(1.0f,1.0f,1.0f);
	colliderData.rotation = XMFLOAT3(0.0f,0.0f,0.0f);
	colliderData.center = position;
	b.colliderData = colliderData;

	//コライダーマネージャーにセット
	ColliderManager::SetCollider(colliderData);

	//弾につける番号を増やす
	number++;
}
