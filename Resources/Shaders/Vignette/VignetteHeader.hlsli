//�}�e���A��
cbuffer ConstBuff : register(b0)
{
	float4 color;
	float strength;
	float2 window;
};

//�ό`�s��
cbuffer ConstBuffTransform : register(b1)
{
	matrix mat;
};

//���_�V�F�[�_�̏o�͍\����
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	//uv
	float2 uv : TEXCOORD;
};