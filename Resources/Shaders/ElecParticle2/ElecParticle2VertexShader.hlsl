#include "ElecParticle2Header.hlsli"

//�G���g���[�|�C���g
VSOutput main(float4 pos:POSITION, float scale:SCALE, float frame:FRAME)
{
	//�s�N�Z���V�F�[�_�ɓn���l
	VSOutput output;
	output.pos = pos;
	output.scale = scale;
    output.frame = frame;

	return output;
}