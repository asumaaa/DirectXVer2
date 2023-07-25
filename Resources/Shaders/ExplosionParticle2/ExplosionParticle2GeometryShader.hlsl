#include "ExplosionParticle2Header.hlsli"

//�l�p�`�̒��_��
static const uint vnum = 4;

//�Z���^�[����I�t�Z�b�g
static const float4 offset_array[vnum] =
{
	float4(-2.5f,-2.5f,0.1f,0.0f),	//����
	float4(-2.5f,+2.5f,0.1f,0.0f),	//����
	float4(+2.5f,-2.5f,0.1f,0.0f),	//�E��
	float4(+2.5f,+2.5f,0.1f,0.0f),	//�E��
};

//uv
static const float2 uv_array[vnum] =
{
	float2(0.0f,1.0f),
	float2(0.0f,0.0f),
	float2(1.0f,1.0f),
	float2(1.0f,0.0f),
};

//�_�̓��͂���l�p�`���o��
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;

	for (uint i = 0; i < vnum; i++)
	{
		//���S����̃I�t�Z�b�g���X�P�[�����O
		float4 offset = offset_array[i] * input[0].scale;
		//���S����̃I�t�Z�b�g���r���{�[�h��]
		offset = mul(matBillboard, offset);
		//���[���h���W�x�[�X�ł��炷
		element.svpos = input[0].pos + offset;
		//�r���[�A�ˉe�ϊ�
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}
}