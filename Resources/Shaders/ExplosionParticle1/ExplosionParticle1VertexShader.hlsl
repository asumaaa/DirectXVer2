#include "ExplosionParticle1Header.hlsli"

//�G���g���[�|�C���g
VSOutput main(float4 pos:POSITION, float scale:SCALE,float4 velocity:VELOCITY)
{
	//�s�N�Z���V�F�[�_�ɓn���l
	VSOutput output;
	output.pos = pos;
	output.scale = scale;
	output.velocity = velocity;

	return output;
}