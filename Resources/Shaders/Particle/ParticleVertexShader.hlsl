#include "ParticleHeader.hlsli"

//�G���g���[�|�C���g
VSOutput main(float4 pos:POSITION)
{
	//�s�N�Z���V�F�[�_�ɓn���l
	VSOutput output;
	output.svpos = pos;

	return output;
}