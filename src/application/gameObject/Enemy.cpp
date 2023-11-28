/**
 * @file Enemy.cpp
 * @brief ゲームオブジェクト 敵
 * @author Asuma Syota
 * @date 2023/4
 */

#include "Enemy.h"
#include "mathOriginal.h"
#include "FbxLoader.h"
#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

Camera* Enemy::camera = nullptr;
Input* Enemy::input = nullptr;
DXInput* Enemy::dxInput = nullptr;

Enemy::~Enemy()
{
	delete modelWait;
	delete objectWait;
}

void Enemy::Initialize()
{
	Sprite* newSprite = new Sprite();
	newSprite->SetTextureNum(4);
	newSprite->Initialize();
	spriteHpBar.reset(newSprite);

	//立っているモデル
	modelWait = FbxLoader::GetInstance()->LoadModelFromFile("enemy");
	//立っているオブジェクト
	objectWait = new FbxObject3D;
	objectWait->Initialize();
	objectWait->SetModel(modelWait);
}

void Enemy::Update()
{
	//動く
	Move();

	//オブジェクト更新
	UpdateObject();

	//スプライト更新
	UpdateSprite();
}

void Enemy::UpdateObject()
{
	objectWait->SetPosition(position);
	objectWait->SetRotation(rotation);
	objectWait->SetScale(scale);

	objectWait->Update();
}

void Enemy::UpdateSprite()
{
	//HPバー
	spriteHpBar->SetAlpha(1.0f);
	spriteHpBar->SetScale({ 100.0f, 100.0 });
	spriteHpBar->SetPosition({ 0.0f, 0.0 });
	spriteHpBar->Update();
}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	objectWait->Draw(cmdList);
}

void Enemy::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	objectWait->DrawLightView(cmdList);
}

void Enemy::DrawSprite(ID3D12GraphicsCommandList* cmdList)
{
	spriteHpBar->Draw(cmdList);
}

void Enemy::Move()
{
	////ジャンプ更新
	//UpdateJump();

	////重力更新
	//UpdateGravity();

	//position.x -= input->PushKey(DIK_A) * speed;
	//position.x += input->PushKey(DIK_D) * speed;
	//position.z -= input->PushKey(DIK_S) * speed;
	//position.z += input->PushKey(DIK_W) * speed;
}

void Enemy::UpdateGravity()
{
	//接地していたらタイマーとベクトルリセット
	if (groundFlag == true)
	{
		fallTimer = 0.0f;
		fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	//接地していなければ
	if (groundFlag == false)
	{
		//落下タイマーが最大値より小さければ
		if (fallTimer < fallTime)
		{
			fallTimer += fallFrame;
		}
	}

	//落下ベクトル計算
	fallVelocity.y = -(GAcceleration * fallTimer);

	//座標に落下ベクトルを加算
	position.x += fallVelocity.x;
	position.y += fallVelocity.y;
	position.z += fallVelocity.z;
}

void Enemy::UpdateJump()
{
	//接地していたら
	if (groundFlag == true)
	{
		//スペースキーでジャンプ
		if (input->TriggerKey(DIK_SPACE))
		{
			groundFlag = false;
			fallTimer = -jumpHeight;
			fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}
}

void Enemy::UpdateAttack()
{
	
}

void Enemy::SetSRV(ID3D12DescriptorHeap* SRV)
{
	objectWait->SetSRV(SRV);
}

void Enemy::HitPlane()
{
	//接地フラグを立てる
	groundFlag = true;

	//めり込まなくなるまで加算
	position.y += 0.1f;

	//オブジェクト更新
	UpdateObject();
}