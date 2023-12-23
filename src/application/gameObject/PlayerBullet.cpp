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
	for (int i = 0; i < bullet.size(); i++)
	{
		bullet[i].timer += 1.0f;
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
	for (int i = 0; i < bullet.size(); i++)
	{	
		//�e�f�[�^����
		bullet[i].colliderData.center = bullet[i].position1;
		bullet[i].colliderData.scale = bullet[i].scale;
	}
}

void PlayerBullet::UpdateParticle()
{
	for (int i = 0; i < bullet.size(); i++)
	{
		//�e�̍��W�Ƀp�[�e�B�N���ǉ�
		particle->Add(bullet[i].position1);
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
	for (int i = 0; i < bullet.size(); i++)
	{
		//�e�̍��W���Z
		if (bullet[i].timer < bullet[i].destoryTime)
		{
			bullet[i].position1 = bullet[i].position1 + bullet[i].velocity;
		}
	}
		//�e���ˌ�
	//	else if(bullet[i].timer < bullet[i].destoryTime1 + bullet[i].destoryTime2)
	//	{
	//		//�ŏ��Ƀx�N�g���擾
	//		if (bullet[i].timer < bullet[i].destoryTime1 + 1)
	//		{
	//			bullet[i].velocity = enemyPos - bullet[i].position1;
	//			bullet[i].velocity = normalize(bullet[i].velocity);

	//			//�ŏ��̒e�̍��W�ۑ�
	//			bullet[i].position2 = bullet[i].position1;
	//			//�v���C���[�̍��W�ۑ�
	//			bullet[i].position3 = enemyPos;
	//		}

	//		//�x�W�G�Ȑ��p���Ԓn�_���v�Z
	//		XMFLOAT3 halfPos = (bullet[i].position3 - bullet[i].position2) / 2.0f;
	//		halfPos.y += 100.0f;
	//		//�x�W�G�Ȑ����v�Z
	//		float t = (bullet[i].timer - bullet[i].destoryTime1) / bullet[i].destoryTime2;
	//		XMFLOAT3 a = lerp(bullet[i].position2, halfPos,easeInCubic(t)* speed);
	//		XMFLOAT3 b = lerp(halfPos, bullet[i].position3, easeInCubic(t) * speed);
	//		bullet[i].position1 = lerp(a, b, easeInCubic(t) * speed);

	//		//�v���C���[����e�̃x�N�g���擾
	//		//XMFLOAT3 velo = playerPos - bullet[i].position;
	//		//velo = normalize(velo);
	//		////�e�̍��W�ɉ��Z
	//		//bullet[i].position = bullet[i].position + velo * speed;
	//	}
	//}
}

void PlayerBullet::CreateBullet()
{
}

void PlayerBullet::DeleteBullet()
{
	//�폜
	for (int i = 0; i < bullet.size(); i++)
	{
		//��莞�Ԍo������
		if (bullet[i].timer == bullet[i].destoryTime)
		{
			//�e�폜
			bullet.erase(bullet.begin() + i);

			continue;
		}

		//�q�b�g�t���O����������
		if (bullet[i].hitFlag == true)
		{
			//�e�폜
			bullet.erase(bullet.begin() + i);
		}
	}
}

void PlayerBullet::SetBullet(XMFLOAT3 position,XMFLOAT3 velocity,float timer,float destoryTime)
{
	//�������珉���l�ݒ�
	Bullet b;
	b.position1 = position;
	b.velocity = velocity;
	b.timer = timer;
	b.destoryTime = destoryTime;
	b.hitFlag = false;

	//�R���C�_�[�̐ݒ�
	//�R���C�_�[���Ƃɖ��O������
	std::string objectName = "enemyBullet" + std::to_string(number);
	//�R���C�_�[�f�[�^�̐���
	JSONLoader::ColliderData colliderData;
	colliderData.type = "Sphere";	//��������̂Ŏ�邽��
	colliderData.objectName = objectName;
	colliderData.scale = b.scale;
	colliderData.rotation = XMFLOAT3(0.0f,0.0f,0.0f);
	colliderData.center = b.position1;
	b.colliderData = colliderData;

	//�R���C�_�[�}�l�[�W���[�ɃZ�b�g
	ColliderManager::SetCollider(colliderData);

	//�ݒ肵���l��}��
	bullet.emplace_back(b);

	//�e�ɂ���ԍ��𑝂₷
	number++;
}
