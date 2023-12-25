/**
 * @file PlayerBullet.cpp
 * @brief �Q�[���I�u�W�F�N�g ���@�̒e
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
	//�^�C�}�[�X�V
	for (std::forward_list<Bullet>::iterator it = bullet.begin(); it != bullet.end(); it++)
	{
		it->timer += 1.0f;
	}

	//�e����������
	DeleteBullet();

	//����
	Move();

	//�R���C�_�[�X�V
	UpdateCollider();

	//�p�[�e�B�N���X�V
	UpdateParticle();
}

void PlayerBullet::UpdateCollider()
{
	for (std::forward_list<Bullet>::iterator it = bullet.begin(); it != bullet.end(); it++)
	{
		//�e�f�[�^����
		it->colliderData.center = it->position1;
		it->colliderData.scale = it->scale;
		it->colliderData.rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
}

void PlayerBullet::UpdateParticle()
{
	for (std::forward_list<Bullet>::iterator it = bullet.begin(); it != bullet.end(); it++)
	{
		//�e�̍��W�Ƀp�[�e�B�N���ǉ�
		particle->Add(it->position1);
	}

	//�X�V
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
	//�X�V
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
	//�������s��������S�폜
	bullet.remove_if([](Bullet& b)
		{
			return b.timer >= b.destoryTime;
		});

	//�q�b�g�t���O���������e��S�폜
	bullet.remove_if([](Bullet& b)
		{
			return b.hitFlag == true;
		});
}

void PlayerBullet::SetHitFlag(int num)
{
	//�擪�̗v�f�擾
	std::forward_list<Bullet>::iterator it = bullet.begin();
	//�q�b�g�t���O�𗧂Ă����v�f�܂Ői�߂�
	for (int i = 0; i < num; i++)
	{
		++it;
	}
	it->hitFlag = true;
}

JSONLoader::ColliderData PlayerBullet::GetColliderData(int num)
{
	//�擪�̗v�f�擾
	std::forward_list<Bullet>::iterator it = bullet.begin();
	//�R���C�_�[���擾�������v�f�܂Ői�߂�
	for (int i = 0; i < num; i++)
	{
		++it;
	}
	return it->colliderData;
}

XMFLOAT3 PlayerBullet::GetPosition(int num)
{
	//�擪�̗v�f�擾
	std::forward_list<Bullet>::iterator it = bullet.begin();
	//���W���擾�������v�f�܂Ői�߂�
	for (int i = 0; i < num; i++)
	{
		++it;
	}
	return it->position1;
}

void PlayerBullet::SetBullet(XMFLOAT3 position,XMFLOAT3 velocity,float timer,float destoryTime)
{
	//�������珉���l�ݒ�
	//���X�g�ɗv�f��ǉ�
	bullet.emplace_front();
	//�ǉ������v�f�̎Q��
	Bullet& b = bullet.front();
	//�l�̃Z�b�g
	b.position1 = position;
	b.scale = XMFLOAT3(3.0f, 3.0f, 3.0f);
	b.velocity = velocity;
	b.timer = timer;
	b.destoryTime = destoryTime;
	b.hitFlag = false;

	//�R���C�_�[�̐ݒ�
	//�R���C�_�[���Ƃɖ��O������
	std::string objectName = "playerBullet" + std::to_string(number);
	//�R���C�_�[�f�[�^�̐���
	JSONLoader::ColliderData colliderData;
	colliderData.type = "Sphere";	//��������̂Ŏ�邽��
	colliderData.objectName = objectName;
	colliderData.scale = XMFLOAT3(1.0f,1.0f,1.0f);
	colliderData.rotation = XMFLOAT3(0.0f,0.0f,0.0f);
	colliderData.center = position;
	b.colliderData = colliderData;

	//�R���C�_�[�}�l�[�W���[�ɃZ�b�g
	ColliderManager::SetCollider(colliderData);

	//�e�ɂ���ԍ��𑝂₷
	number++;
}
