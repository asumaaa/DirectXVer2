#pragma once

#include "DirectXMath.h"
using namespace DirectX;

struct Matrix4
{
	float m[4][4];
};

Matrix4 identity();
Matrix4 scale(const XMFLOAT3& s);

Matrix4 rotationX(float angle);
Matrix4 rotationY(float angle);
Matrix4 rotationZ(float angle);

Matrix4 translate(const XMFLOAT3& t);

XMFLOAT3 transform(const XMFLOAT3& v, const Matrix4& m);

Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
XMFLOAT3 operator*(const XMFLOAT3& v, const Matrix4& m);
