#pragma once
#include "DirectXMath.h"
#define PI 3.14159265359

#pragma region �֗��֐�

//�����_���Œl��Ԃ��֐�
int RNG(int min, int max, bool preciseMode = false);

#pragma endregion

#pragma region �C�[�W���O

//�C�[�W���O�֐�:easeInSine
float easeInSine(float x);
//�C�[�W���O�֐�:easeOutSine
float easeOutSine(float x);
//�C�[�W���O�֐�:easeInOutSine
float easeInOutSine(float x);
//�C�[�W���O�֐�:easeInQuad
float easeInQuad(float x);
//�C�[�W���O�֐�:easeOutQuad
float easeOutQuad(float x);
//�C�[�W���O�֐�:easeInOutQuad
float easeInOutQuad(float x);
//�C�[�W���O�֐�:easeInCubic
float easeInCubic(float x);
//�C�[�W���O�֐�:easeOutCubic
float easeOutCubic(float x);
//�C�[�W���O�֐�:easeInOutCubic
float easeInOutCubic(float x);
//�C�[�W���O�֐�:easeInQuart
float easeInQuart(float x);
//�C�[�W���O�֐�:easeOutQuart
float easeOutQuart(float x);
//�C�[�W���O�֐�:easeInOutQuart
float easeInOutQuart(float x);

#pragma endregion

#pragma region XMFLOAT3�֘A

//���Z
const DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
//���Z
const DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
//��Z
const DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 v, float s);
//���Z
const DirectX::XMFLOAT3 operator/(DirectX::XMFLOAT3 v, float s);
//����
float dot(DirectX::XMFLOAT3 v1,DirectX::XMFLOAT3 v2);
//�O��
DirectX::XMFLOAT3 cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
//����
float length(DirectX::XMFLOAT3 v);
//���K��
DirectX::XMFLOAT3 normalize(DirectX::XMFLOAT3 v);
//�������ɓ��e���ꂽ���������瓊�e���������Z�o
float LenSegOnSeparateAxis(DirectX::XMFLOAT3 Sep, DirectX::XMFLOAT3 e1, 
	DirectX::XMFLOAT3 e2, DirectX::XMFLOAT3 e3 = { 0,0,0 });
//velocity�̌�����rotation�����킹��
DirectX::XMFLOAT3 rollRotation(DirectX::XMFLOAT3 vector,DirectX::XMFLOAT3 rotation);

#pragma endregion