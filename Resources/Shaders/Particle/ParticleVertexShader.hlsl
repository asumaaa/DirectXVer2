#include "ParticleHeader.hlsli"

//�G���g���[�|�C���g
VSOutput main(float4 pos:POSITION, float scale:SCALE)
{
	//�s�N�Z���V�F�[�_�ɓn���l
	VSOutput output;
	output.svpos = pos;
	output.scale = scale;

	return output;
}