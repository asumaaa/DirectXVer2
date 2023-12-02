/**
 * @file EnemyBullet.cpp
 * @brief �Q�[���I�u�W�F�N�g �G�̒e
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
		//�e�f�[�^����
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
	//�������珉���l�ݒ�
	Bullet b;
	b.position = position;
	b.rotation = baseRotation;
	b.scale = baseScale;
	b.velocity = velocity;
	b.timer = 0.0f;
	b.hitFlag = false;

	//�R���C�_�[�̐ݒ�
	//�R���C�_�[���Ƃɖ��O������
	std::string objectName = "enemyBullet" + std::to_string(number);
	//�R���C�_�[�f�[�^�̐���
	JSONLoader::ColliderData colliderData;
	colliderData.type = "Sphere";	//��������̂Ŏ�邽��
	colliderData.objectName = objectName;
	colliderData.scale = b.scale;
	colliderData.rotation = b.rotation;
	colliderData.center = b.position;
	b.colliderData = colliderData;

	//�R���C�_�[�}�l�[�W���[�ɃZ�b�g
	ColliderManager::SetCollider(colliderData);

	//�ݒ肵���l��}��
	bullet.emplace_back(b);

	//�e�ɂ���ԍ��𑝂₷
	number++;
}
