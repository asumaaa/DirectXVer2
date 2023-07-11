#include "Collision.h"

using namespace DirectX;

bool Collision::CheckSphere2Plane(const CollisionPrimitive::Sphere& sphere, const CollisionPrimitive::Plane& plane, XMVECTOR* inter)
{
    //���W�h�̌��_���狅�̒��S���W�ւ̋���
    XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
    //���ʂ̌��_���������Y���邱�Ƃŕ��ʂƋ��̒��S�Ƃ̋������o��
    float dist = distV.m128_f32[0] - plane.distance;
    //�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
    if (fabsf(dist) > sphere.redius) return false;

    //�^�������_���v�Z
    if (inter)
    {
        //���ʏ�̐ړ_���A�c����_�Ƃ���
        *inter = -dist * plane.normal + sphere.center;
    }

    return true;
}
