#include "ExplosionParticle1Header.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);
//�G���g���[�|�C���g
float4 main(GSOutput input) : SV_TARGET
{
	//�摜�̃f�[�^
	float4 texData = float4(tex.Sample(smp,input.uv));

	//RBG�l�𔽓]
	/*texData = float4(1.0f - texData.x, 1.0f - texData.y, 1.0f - texData.z, 1.0f);*/

	//�A���t�@�l������
	//return float4(texData.x * 0.1, texData.y * 0.1, texData.z * 0.1, 1.0f);
	return float4(texData.x * 0.992, texData.y * 0.3f, 0.0f, 1.0f);
}