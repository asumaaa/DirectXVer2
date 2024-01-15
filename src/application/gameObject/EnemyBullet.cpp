/**
 * @file EnemyBullet.cpp
 * @brief �Q�[���I�u�W�F�N�g �G�̒e
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

	//�e����������
	DeleteBullet();

	//����
	Move();

	//�R���C�_�[�X�V
	UpdateCollider();

	//�p�[�e�B�N���X�V
	UpdateParticle();
}

void EnemyBullet::UpdateCollider()
{
	for (int i = 0; i < bullet.size(); i++)
	{	
		//�e�f�[�^����
		bullet[i].colliderData.center = bullet[i].position1;
		bullet[i].colliderData.scale = bullet[i].scale;
		bullet[i].colliderData.rotation = bullet[i].rotation;
	}
}

void EnemyBullet::UpdateParticle()
{
	for (int i = 0; i < bullet.size(); i++)
	{
		//�e�̍��W�Ƀp�[�e�B�N���ǉ�
		particle->Add(bullet[i].position1);
	}

	//�X�V
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
	//�X�V
	particle->Draw(cmdList);
}

void EnemyBullet::Move()
{
	for (int i = 0; i < bullet.size(); i++)
	{
		//���߃��[�V������
		if (bullet[i].timer < bullet[i].destoryTime1)
		{
			//if (bullet[i].timer == 0)
			//{
			//	//�ŏ��̍��W�ۑ�
			//	bullet[i].position2 = bullet
			//}
			/*bullet[i].position1 = XMFLOAT3(-20, 40, 50);*/
		}
		//�e���ˌ�
		else if(bullet[i].timer < bullet[i].destoryTime1 + bullet[i].destoryTime2)
		{
			//�ŏ��Ƀx�N�g���擾
			if (bullet[i].timer < bullet[i].destoryTime1 + 1)
			{
				bullet[i].velocity = playerPos - bullet[i].position1;
				bullet[i].velocity = normalize(bullet[i].velocity);

				//�ŏ��̒e�̍��W�ۑ�
				bullet[i].position2 = bullet[i].position1;
				//�v���C���[�̍��W�ۑ�
				bullet[i].position3 = playerPos;
			}

			//�x�W�G�Ȑ��p���Ԓn�_���v�Z
			XMFLOAT3 halfPos = (bullet[i].position3 - bullet[i].position2) / 2.0f;
			halfPos.y += 100.0f;
			//�x�W�G�Ȑ����v�Z
			float t = (bullet[i].timer - bullet[i].destoryTime1) / bullet[i].destoryTime2;
			XMFLOAT3 a = lerp(bullet[i].position2, halfPos,easeInCubic(t)* speed);
			XMFLOAT3 b = lerp(halfPos, bullet[i].position3, easeInCubic(t) * speed);
			bullet[i].position1 = lerp(a, b, easeInCubic(t) * speed);

			//�v���C���[����e�̃x�N�g���擾
			//XMFLOAT3 velo = playerPos - bullet[i].position;
			//velo = normalize(velo);
			////�e�̍��W�ɉ��Z
			//bullet[i].position = bullet[i].position + velo * speed;
		}
	}
}

void EnemyBullet::CreateBullet()
{
}

void EnemyBullet::DeleteBullet()
{
	//�폜
	for (int i = 0; i < bullet.size(); i++)
	{
		//��莞�Ԍo������
		if (bullet[i].timer == bullet[i].destoryTime1 + bullet[i].destoryTime2)
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

void EnemyBullet::SetBullet(XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 lastScale, float timer, float destoryTime1, float destoryTime2)
{
	//�������珉���l�ݒ�
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

	//�R���C�_�[�̐ݒ�
	//�R���C�_�[���Ƃɖ��O������
	std::string objectName = "enemyBullet" + std::to_string(number);
	//�R���C�_�[�f�[�^�̐���
	JSONLoader::ColliderData colliderData;
	colliderData.type = "Sphere";	//��������̂Ŏ�邽��
	colliderData.objectName = objectName;
	colliderData.scale = b.scale;
	colliderData.rotation = b.rotation;
	colliderData.center = b.position1;
	b.colliderData = colliderData;

	//�R���C�_�[�}�l�[�W���[�ɃZ�b�g
	ColliderManager::SetCollider(colliderData);

	//�ݒ肵���l��}��
	bullet.emplace_back(b);

	//�e�ɂ���ԍ��𑝂₷
	number++;
}
