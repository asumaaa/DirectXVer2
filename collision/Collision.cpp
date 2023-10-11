#include "Collision.h"

using namespace DirectX;

bool Collision::CheckSphere2Plane(const CollisionPrimitive::Sphere& sphere, const CollisionPrimitive::Plane& plane, XMVECTOR* inter)
{
    //座標敬の原点から球の中心座標への距離
    XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
    //平面の原点距離を減産することで平面と球の中心との距離が出る
    float dist = distV.m128_f32[0] - plane.distance;
    //距離の絶対値が半径より大きければ当たっていない
    if (fabsf(dist) > sphere.redius) return false;

    //疑似交差点を計算
    if (inter)
    {
        //平面上の接点を、議事交点とする
        *inter = -dist * plane.normal + sphere.center;
    }

    return true;
}