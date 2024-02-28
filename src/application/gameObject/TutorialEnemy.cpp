#include "TutorialEnemy.h"
#include "mathOriginal.h"
#include "FbxLoader.h"
#include "imgui.h"
#include "ColliderManager.h"
#define G 6.674	//���L���͒萔
#define GAcceleration 9.80665 * 1/10	//�d�͉����x

Camera* TutorialEnemy::camera = nullptr;
Input* TutorialEnemy::input = nullptr;

void TutorialEnemy::Initialize()
{
	//HP�o�[�̃X�v���C�g
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

	//���f��
	model = FbxLoader::GetInstance()->LoadModelFromFile("Tree");
	//�I�u�W�F�N�g
	object = new FbxObject3D;
	object->Initialize();
	object->SetModel(model);
	object->SetTextureNum(0);

	//�R���C�_�[�̐ݒ�
	colliderData.type = "Sphere";	//��������̂Ŏ�邽��
	colliderData.objectName = "tutorialEnemy";
	colliderData.scale = scale;
	colliderData.rotation = rotation;
	colliderData.center = position;
	//�R���C�_�[�}�l�[�W���[�ɃZ�b�g
	ColliderManager::SetCollider(colliderData);
}

void TutorialEnemy::Update()
{
	//�I�u�W�F�N�g�X�V
	UpdateObject();

	//�R���C�_�[�X�V
	UpdateCollider();

	//�X�v���C�g�X�V
	UpdateSprite();

	//�_���[�W�X�V
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
	//HP�o�[�����݂�HP��
	hpBar2Scale.x = hpBar2OriginalScale.x * (HP / maxHP);
	hpBar3Pos.x = hpBar3OriginalPos.x - (hpBar2OriginalScale.x * ((maxHP - HP) / maxHP));

	//�X�V
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
	//���U�������������
	if (HitFlag1 == true)
	{
		//HP�����炷
		HP -= 1.0f;
	}

	//���S
	if (HP <= 0)
	{
		isDead = true;
	}

	//�t���O�����Ƃɖ߂�
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
	//��e�t���O�𗧂Ă�
	HitFlag1 = true;
}
