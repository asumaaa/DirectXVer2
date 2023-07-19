#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "DXInput.h"
#include "WinApp.h"
//#include "Player.h"
#define PI 3.14159265359

using namespace DirectX;

class Camera
{
public:

public:
	//�V���O���g���C���X�^���X
	static Camera* GetInstance();
	static void SetInput(Input* input) { Camera::input = input; }
	static void SetDXInput(DXInput* dxInput) { Camera::dxInput = dxInput; }
	//�C���X�g���N�^ �f�X�g���N�^
	Camera();
	~Camera();
	//������
	void Initialize();
	//�X�V
	void Update();
	//�r���{�[�h�s��̍X�V
	void BillboardUpdate();
	//�f�o�b�O Arrow�L�[�Ŏ��_���W�ύX
	void DebugUpdate();
	//�v���C���[�ǔ�
	void UpdatePlayer(XMFLOAT3 playerPos,XMFLOAT3 playerRot);

	//�Q�b�^�[�Z�b�^�[
	void SetTarget(XMFLOAT3 pos);
	void SetEye(XMFLOAT3 pos);
	XMFLOAT3 GetEye() { return eye_; };
	XMFLOAT3 GetTarget() { return target_; };
	XMFLOAT3 GetUp() { return up_; };
	XMMATRIX GetMatProjection() { return matProjection_; };
	XMMATRIX GetMatView() { return matView_; };
	XMMATRIX GetMatViewProjection() { return matView_ * matProjection_; };
	XMMATRIX GetMatBillboard() { return matBillboard_; }

private:
	//����
	//�L�[�{�[�h
	static Input* input;
	//�R���g���[���[
	static DXInput* dxInput;
	//�ˉe�ϊ�
	XMMATRIX matProjection_ = XMMatrixIdentity();
	//�r���[�ϊ��s��
	XMMATRIX matView_ = XMMatrixIdentity();
	XMFLOAT3 eye_ = { 0, 20, -100 };
	XMFLOAT3 target_ = { 0, 0, 0 };
	XMFLOAT3 up_ = { 0, 1, 0 };
	//�r���{�[�h�s��
	XMMATRIX matBillboard_ = XMMatrixIdentity();

	//�^�[�Q�b�g�܂ł̋���
	float DebugTargetDistance = 20.0f;
	//���Z�p
	float DebugChangeRot = (float)PI;
	float DebugChangeRot2 = 0.5f;
	float DebugChangeDistance = 0.0f;

	//�v���C���[
	//�^�[�Q�b�g�܂ł̋���
	float playerTargetDistance = 20.0f;
	//���Z�p
	float playerChangeRot = (float)PI;
	float playerChangeRot2 = 0.5f;
	float playerChangeDistance = 0.0f;
};
