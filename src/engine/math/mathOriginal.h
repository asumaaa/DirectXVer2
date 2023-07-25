#pragma once
#include "DirectXMath.h"
#define PI 3.14159265359

#pragma region 便利関数

//ランダムで値を返す関数
int RNG(int min, int max, bool preciseMode = false);

#pragma endregion

#pragma region イージング

//イージング関数:easeInSine
float easeInSine(float x);
//イージング関数:easeOutSine
float easeOutSine(float x);
//イージング関数:easeInOutSine
float easeInOutSine(float x);
//イージング関数:easeInQuad
float easeInQuad(float x);
//イージング関数:easeOutQuad
float easeOutQuad(float x);
//イージング関数:easeInOutQuad
float easeInOutQuad(float x);
//イージング関数:easeInCubic
float easeInCubic(float x);
//イージング関数:easeOutCubic
float easeOutCubic(float x);
//イージング関数:easeInOutCubic
float easeInOutCubic(float x);
//イージング関数:easeInQuart
float easeInQuart(float x);
//イージング関数:easeOutQuart
float easeOutQuart(float x);
//イージング関数:easeInOutQuart
float easeInOutQuart(float x);
//イージング関数:easeInQuint
float easeInQuint(float x);
//イージング関数:easeOutQuint
float easeOutQuint(float x);
//イージング関数:easeInOutQuint
float easeInOutQuint(float x);
//イージング関数:easeInExpo
float easeInExpo(float x);
//イージング関数:easeOutExpo
float easeOutExpo(float x);
//イージング関数:easeInOutExpo
float easeInOutExpo(float x);
//イージング関数:easeInCirc
float easeInCirc(float x);
//イージング関数:easeOutCirc
float easeOutCirc(float x);
//イージング関数:easeInOutCirc
float easeInOutCirc(float x);
//イージング関数:easeInBack
float easeInBack(float x);
//イージング関数:easeOutBack
float easeOutBack(float x);
//イージング関数:easeInOutBack
float easeInOutBack(float x);

//イージング関数:easeInElastic
float easeInElastic(float x);
//イージング関数:easeOutElastic
float easeOutElastic(float x);
//イージング関数:easeInOutElastic
float easeInOutElastic(float x);
//イージング関数:easeInBounce
float easeInBounce(float x);
//イージング関数:easeOutBounce
float easeOutBounce(float x);
//イージング関数:easeInOutBounce
float easeInOutBounce(float x);

#pragma endregion

#pragma region XMFLOAT3関連

//加算
const DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
//減算
const DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
//乗算
const DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 v, float s);
//除算
const DirectX::XMFLOAT3 operator/(DirectX::XMFLOAT3 v, float s);
//内積
float dot(DirectX::XMFLOAT3 v1,DirectX::XMFLOAT3 v2);
//外積
DirectX::XMFLOAT3 cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
//長さ
float length(DirectX::XMFLOAT3 v);
//正規化
DirectX::XMFLOAT3 normalize(DirectX::XMFLOAT3 v);
//分離軸に投影された軸成分から投影線分長を算出
float LenSegOnSeparateAxis(DirectX::XMFLOAT3 Sep, DirectX::XMFLOAT3 e1, 
	DirectX::XMFLOAT3 e2, DirectX::XMFLOAT3 e3 = { 0,0,0 });
//velocityの向きにrotationを合わせる
DirectX::XMFLOAT3 rollRotation(DirectX::XMFLOAT3 vector,DirectX::XMFLOAT3 rotation);

#pragma endregion