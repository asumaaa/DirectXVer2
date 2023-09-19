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
