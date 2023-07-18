#include "BillboardSpriteHeader.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);
//�G���g���[�|�C���g
float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
	float4 texcoord = tex.Sample(smp,input.uv);

	//�e�N�X�`���������ȏ��̏ꍇ
	if (texcoord.w == 0)
	{
		discard;
	}

	return float4(texcoord.x, texcoord.y, texcoord.z,1);
}