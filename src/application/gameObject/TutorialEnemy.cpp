#include "TutorialEnemy.h"
#include "mathOriginal.h"
#include "FbxLoader.h"
#include "imgui.h"
#include "ColliderManager.h"
#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

Camera* TutorialEnemy::camera = nullptr;
Input* TutorialEnemy::input = nullptr;

void TutorialEnemy::Initialize()
{
	//HPバーのスプライト
	hpBar1 = new Sprite();
	hpBar1->Initialize();
	hpBar1->SetTextureNum(23);
	hpBar2 = new Sprite();
	hpBar2->Initialize();
	hpBar2->SetTextureNum(24);
	hpBar3 = new Sprite();
	hpBar3->Initialize();
	hpBar3->SetTextureNum(25);
	hpBar4 = new Sprite();
	hpBar4->Initialize();
	hpBar4->SetTextureNum(26);

	//モデル
	model = FbxLoader::GetInstance()->LoadModelFromFile("Tree");
	//オブジェクト
	object = new FbxObject3D;
	object->Initialize();
	object->SetModel(model);
	object->SetTextureNum(0);

	//コライダーの設定
	colliderData.type = "Sphere";	//判定を球体で取るため
	colliderData.objectName = "tutorialEnemy";
	colliderData.scale = scale;
	colliderData.rotation = rotation;
	colliderData.center = position;
	//コライダーマネージャーにセット
	ColliderManager::SetCollider(colliderData);
}

void TutorialEnemy::Update()
{
	//オブジェクト更新
	UpdateObject();

	//コライダー更新
	UpdateCollider();

	//スプライト更新
	UpdateSprite();

	//ダメージ更新
	UpdateDamage();
}

void TutorialEnemy::UpdateObject()
{
	object->SetPosition(position);
	object->SetRotation(rotation);
	object->SetScale(scale);

	object->Update();
}

void TutorialEnemy::UpdateSprite()
{
	//HPバーを現在のHPに
	hpBar2Scale.x = hpBar2OriginalScale.x * (HP / maxHP);
	hpBar3Pos.x = hpBar3OriginalPos.x - (hpBar2OriginalScale.x * ((maxHP - HP) / maxHP));

	//更新
	hpBar1->Update(hpBar1Pos, hpBar1Scale);
	hpBar2->Update(hpBar2Pos, hpBar2Scale);
	hpBar3->Update(hpBar3Pos, hpBar3Scale);
	hpBar4->Update(hpBar4Pos, hpBar4Scale);
}

void TutorialEnemy::UpdateCollider()
{
	colliderData.scale = XMFLOAT3(20.0f, 10.0f, 10.0f);
	colliderData.rotation = rotation;
	colliderData.center = position + XMFLOAT3(0.0f, 5.0f, 0.0f);
}

void TutorialEnemy::UpdateDamage()
{
	//炎攻撃をくらった際
	if (HitFlag1 == true)
	{
		//HPを減らす
		HP -= 1.0f;
	}

	//死亡
	if (HP <= 0)
	{
		isDead = true;
	}

	//フラグをもとに戻す
	HitFlag1 = false;
}

void TutorialEnemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object->Draw(cmdList);
}

void TutorialEnemy::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	object->DrawLightView(cmdList);
}

void TutorialEnemy::DrawSprite(ID3D12GraphicsCommandList* cmdList)
{
	hpBar2->Draw(cmdList);
	hpBar4->Draw(cmdList);
	hpBar1->Draw(cmdList);
	hpBar3->Draw(cmdList);
}

void TutorialEnemy::Move()
{
}

void TutorialEnemy::StatusManager()
{
}

void TutorialEnemy::SetSRV(ID3D12DescriptorHeap* SRV)
{
	object->SetSRV(SRV);
}

void TutorialEnemy::Reset()
{
	HP = maxHP;
	isDead = false;
}

void TutorialEnemy::HitBullet1()
{
	//被弾フラグを立てる
	HitFlag1 = true;
}
