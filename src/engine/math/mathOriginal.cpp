#include "mathOriginal.h"
#include "cmath"
#include "Matrix4.h"

float easeOutQuart(float x)
{
	return 1 - pow(1 - x, 4);
}

float easeInOutQuart(float x)
{
	return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
}

float easeInQuint(float x)
{
	return x * x * x * x * x;
}

float easeOutQuint(float x)
{
	return 1 - pow(1 - x, 5);
}

float easeInOutQuint(float x)
{
	return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
}

float easeInExpo(float x)
{
	return x == 0 ? 0 : pow(2, 10 * x - 10);
}

float easeOutExpo(float x)
{
	return x == 1 ? 1 : 1 - pow(2, -10 * x);
}

float easeInOutExpo(float x)
{
	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5 ? pow(2, 20 * x - 10) / 2
		: (2 - pow(2, -20 * x + 10)) / 2;
}

float easeInCirc(float x)
{
	return 1 - sqrt(1 - pow(x, 2));
}

float easeOutCirc(float x)
{
	return sqrt(1 - pow(x - 1, 2));
}

float easeInOutCirc(float x)
{
	return x < 0.5
		? (1 - sqrt(1 - pow(2 * x, 2))) / 2
		: (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
}

float easeInBack(float x)
{
	float c1 = 1.70158;
	float c3 = c1 + 1;

	return c3 * x * x * x - c1 * x * x;
}

float easeOutBack(float x)
{
	float c1 = 1.70158;
	float c3 = c1 + 1;

	return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}

float easeInOutBack(float x)
{
	float c1 = 1.70158;
	float c2 = c1 * 1.525;

	return x < 0.5
		? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
		: (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

float easeInElastic(float x)
{
	float c4 = (2 * PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
}

float easeOutElastic(float x)
{
	float c4 = (2 * PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
}

float easeInOutElastic(float x)
{
	float c5 = (2 * PI) / 4.5;

	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5
		? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
		: (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
}

float easeInBounce(float x)
{
	return 1 - easeOutBounce(1 - x);
}

float easeOutBounce(float x)
{
	float n1 = 7.5625;
	float d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}

float easeInOutBounce(float x)
{
	return x < 0.5
		? (1 - easeOutBounce(1 - 2 * x)) / 2
		: (1 + easeOutBounce(2 * x - 1)) / 2;
}

int RNG(int min, int max, bool preciseMode)
{
	if (!preciseMode) {
		//return (rand() % (max + 1 - min) + min);
		return rand() / RAND_MAX * max;
	}

	int ret = 0;
	do {
		ret = rand();
	} while (ret >= RAND_MAX - RAND_MAX % (max + 1 - min));
	ret = ret % (max + 1 - min) + min;
}

float easeInSine(float x)
{
	return 1 - cos((x * PI) / 2);
}

float easeOutSine(float x)
{
	return sin((x * PI) / 2);
}

float easeInOutSine(float x)
{
	return -(cos(PI * x) - 1) / 2;
}

float easeInQuad(float x)
{
	return x * x;
}

float easeOutQuad(float x)
{
	return 1 - (1 - x) * (1 - x);
}

float easeInOutQuad(float x)
{
	return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
}

float easeInCubic(float x)
{
	return x * x * x;
}

float easeInQuart(float x)
{
	return x * x * x * x;
}

float easeOutCubic(float x)
{
	return 1 - pow(1 - x, 3);
}

float easeInOutCubic(float x)
{
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

const DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
	return DirectX::XMFLOAT3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

const DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
	return DirectX::XMFLOAT3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

const DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 v, float s)
{
	return DirectX::XMFLOAT3(v.x * s, v.y * s, v.z * s);
}

const DirectX::XMFLOAT3 operator/(DirectX::XMFLOAT3 v, float s)
{
	return DirectX::XMFLOAT3(v.x / s, v.y / s, v.z / s);
}

float dot( DirectX::XMFLOAT3 v1,DirectX::XMFLOAT3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

DirectX::XMFLOAT3 cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
	return DirectX::XMFLOAT3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

float length(DirectX::XMFLOAT3 v)
{
	float x2 = v.x * v.x;
	float y2 = v.y * v.y;
	float z2 = v.z * v.z;
	return sqrt(x2 + y2 + z2);
}

DirectX::XMFLOAT3 normalize(DirectX::XMFLOAT3 v)
{
	float len = length(v);

	return v / len;
}

float LenSegOnSeparateAxis(DirectX::XMFLOAT3 Sep, DirectX::XMFLOAT3 e1, DirectX::XMFLOAT3 e2, DirectX::XMFLOAT3 e3)
{
	float r1 = fabs(dot(Sep, e1));
	float r2 = fabs(dot(Sep, e2));
	float r3 = fabs(dot(Sep, e3));
	return r1 + r2 + r3;
}

DirectX::XMFLOAT3 rollRotation(DirectX::XMFLOAT3 vector, DirectX::XMFLOAT3 rotation)
{
	Matrix4 matRotX, matRotY, matRotZ;
	matRotX = rotationX(rotation.x);
	matRotY = rotationY(rotation.y);
	matRotZ = rotationZ(rotation.z);

	matRotZ *= matRotX;
	matRotZ *= matRotY;

	DirectX::XMFLOAT3 v;

	v.x = vector.x * matRotZ.m[0][0] + vector.y * matRotZ.m[1][0] + vector.z * matRotZ.m[2][0];
	v.y = vector.x * matRotZ.m[0][1] + vector.y * matRotZ.m[1][1] + vector.z * matRotZ.m[2][1];
	v.z = vector.x * matRotZ.m[0][2] + vector.y * matRotZ.m[1][2] + vector.z * matRotZ.m[2][2];

	return v;
}
