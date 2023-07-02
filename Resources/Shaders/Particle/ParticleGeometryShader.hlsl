#include "ParticleHeader.hlsli"

//�l�p�`�̒��_��
static const uint vnum = 4;

//�Z���^�[����I�t�Z�b�g
static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f,0.0f,0.0f),	//����
	float4(-0.5f,+0.5f,0.0f,0.0f),	//����
	float4(+0.5f,-0.5f,0.0f,0.0f),	//�E��
	float4(+0.5f,+0.5f,0.0f,0.0f),	//�E��
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
	//4�_���܂킷
	for (uint i = 0; i < vnum; i++)
	{
		//���[���h���W�x�[�X�ł��炷
		element.svpos = input[0].svpos + offset_array[i];
		//�r���[�A�ˉe�Ԋ�
		element.svpos = mul(mul(viewproj, world), element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}
}