#include "SparkParticle2Header.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);
//�G���g���[�|�C���g
float4 main(GSOutput input) : SV_TARGET
{
	//�摜�̃f�[�^
	float4 texData = float4(tex.Sample(smp,input.uv));

	//�A���t�@�l������
	return float4(texData.x, texData.y, texData.z, 1);
}