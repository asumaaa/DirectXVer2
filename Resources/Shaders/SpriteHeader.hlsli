//�}�e���A��
cbuffer ConstBuff : register(b0)
{
	float4 color;
};

//���_�V�F�[�_�̏o�͍\����
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	//uv
	float2 uv : TEXCOORD;
};