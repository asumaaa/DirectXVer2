cbuffer cbuff0 : register(b0)
{
	matrix viewproj;	//�r���[�v���W�F�N�V�����s��
	matrix world;	//���[���h�s��
}

//���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
};

//�W�I���g���V�F�[�_����s�N�Z���V�F�[�_�ւ̏o��
struct GSOutput
{
	float4 svpos:SV_POSITION;	//�V�X�e���p���_���W
	float2 uv:TEXCOORD;	//uv�l
};