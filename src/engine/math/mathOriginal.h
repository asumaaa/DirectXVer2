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
//�C�[�W���O�֐�:easeInQuint
float easeInQuint(float x);
//�C�[�W���O�֐�:easeOutQuint
float easeOutQuint(float x);
//�C�[�W���O�֐�:easeInOutQuint
float easeInOutQuint(float x);
//�C�[�W���O�֐�:easeInExpo
float easeInExpo(float x);
//�C�[�W���O�֐�:easeOutExpo
float easeOutExpo(float x);
//�C�[�W���O�֐�:easeInOutExpo
float easeInOutExpo(float x);
//�C�[�W���O�֐�:easeInCirc
float easeInCirc(float x);
//�C�[�W���O�֐�:easeOutCirc
float easeOutCirc(float x);
//�C�[�W���O�֐�:easeInOutCirc
float easeInOutCirc(float x);
//�C�[�W���O�֐�:easeInBack
float easeInBack(float x);
//�C�[�W���O�֐�:easeOutBack
float easeOutBack(float x);
//�C�[�W���O�֐�:easeInOutBack
float easeInOutBack(float x);

//�C�[�W���O�֐�:easeInElastic
float easeInElastic(float x);
//�C�[�W���O�֐�:easeOutElastic
float easeOutElastic(float x);
//�C�[�W���O�֐�:easeInOutElastic
float easeInOutElastic(float x);
//�C�[�W���O�֐�:easeInBounce
float easeInBounce(float x);
//�C�[�W���O�֐�:easeOutBounce
float easeOutBounce(float x);
//�C�[�W���O�֐�:easeInOutBounce
float easeInOutBounce(float x);

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