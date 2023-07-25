#pragma once
#include "memory"
#include "list"

#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "FireParticle.h"
#include "Light.h"
#include "LightGroup.h"
#include "CSVLoader.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "ColliderCubeObject.h"
#include "ColliderSphereObject.h"
#include "ColliderPlaneObject.h"
#include "ColliderManager.h"
#include "SparkParticle.h"
#include "SparkParticle2.h"
#include "ExplosionParticle1.h"
#include "ExplosionParticle2.h"
#include "BillboardSprite.h"
#include "BillboardSpriteModel.h"

class GameScene
{
	//�����o�֐�
public:
	GameScene();
	~GameScene();
	//������
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//�I����
	void Finalize();
	//�X�V
	void Update();
	void UpdateCollider();
	//�`��
	void Draw();
	void DrawFBXLightView();
	void DrawFBX();
	void DrawCollider();
	void DrawSprite();
	void DrawParticle();
	
	//�Z�b�^�[
	void SetSRV(ID3D12DescriptorHeap* SRV);
	//�Q�b�^�[
	DirectX::XMMATRIX GetLightViewProjection();

	//�����o�ϐ�
private:
	//�f�o�C�X��input
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	//�J����
	std::unique_ptr<Camera> camera_;

	//fbx
	std::list<std::unique_ptr<FbxModel>> models;

	//���x���G�f�B�^
	std::unique_ptr<JSONLoader> jsonLoader;

	//�I�u�W�F�N�g
	std::list<std::unique_ptr<FbxObject3D>> object;

	//���C�g �e�p
	std::unique_ptr<Light> light;
	float lightDir[3] = {0.0f,-1.0f , -1.0f};
	float lightPos[3] = {0.0f,25.0f,25.0f};
	float lightTarget[3] = { 0.0f,0.0f,0.0f };
	float lightFactorAngle[2] = { 20.0f,30.0f, };
	float lightAtten[3] = {0.0f,0.0f,0.0f};

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float lightManagerDir[3] = { 0.0f,-1.0f , 1.0f };

	//�e�N�X�`���}�l�[�W���[
	std::unique_ptr <TextureManager> textureManager;

	//�ό`�s��
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };

	//�v���C���[
	std::unique_ptr<Player> player;
	//�v���C���[�̒e
	std::unique_ptr<PlayerBullet>playerBullet;

	//�G
	std::unique_ptr<Enemy>enemy;
	
	//����
	/*std::unique_ptr<Plane> plane;*/

	//�R���C�_�[�̃��f��
	std::unique_ptr<ColliderCubeModel>colliderCubeModel;
	std::unique_ptr<ColliderSphereModel>colliderSphereModel;
	std::unique_ptr<ColliderPlaneModel>colliderPlaneModel;

	//�R���C�_�[
	std::unique_ptr<ColliderManager> colliderManager;

	//�e����p�[�e�B�N��
	std::unique_ptr<SparkParticle>sparkParticle;

	//�e����p�[�e�B�N��2
	std::unique_ptr<SparkParticle2>sparkParticle2;

	//�����p�[�e�C�N��1
	std::unique_ptr<ExplosionParticle1>explosionParticle1;

	//�����p�[�e�C�N��2
	std::unique_ptr<ExplosionParticle2>explosionParticle2;

	//�`��t���O
	int drawParticle[1] = { 1 };
	int drawFbx[1] = { 1 };
	int drawSprite[1] = { 0 };
	int drawCollider[1] = { 1 };

	//�r���{�[�h�@
	std::unique_ptr<BillboardSprite>billboardSprite;
	std::unique_ptr<BillboardSpriteModel>billboardSpriteModel;
};
