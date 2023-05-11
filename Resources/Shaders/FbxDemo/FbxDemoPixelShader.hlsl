#include "FbxDemoHeader.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

struct PSOutput 
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input) : SV_TARGET
{
	PSOutput output;
	//�e�N�X�`���}�b�s���O
	float4 texcoord = tex.Sample(smp,input.uv);
	//���C�g�̌���
	float3 lightdir = float3(1, -1, 1);
	//���C�g�̐F
	float3 lightcolor = float3(1, 1, 1);
	//���_���W���Œ�
	float eyepos = float3(0, 10, 10);
	//Lambert����
	float3 light = normalize(lightdir);	//�E���� �����̃��C�g
	float diffuse0 = saturate(dot(-light, input.normal));
	float brightness = diffuse0 + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	
	//Phong����
	//�����ˌ�
	//float3 ambient0 = ambient;
	////�g�U���ˌ�
	//float3 diffuse0 = dot(-lightdir, input.normal) * diffuse;
	////����
	//const float shininess = 4.0f;
	////���_���王�_�ւ̃x�N�g��
	//const float3 eyedir = normalize(eyepos - input.pos.xyz);
	////���ˌ��x�N�g��
	//float3 reflect0 = normalize(lightdir + 2 * dot(-lightdir, input.normal) * input.normal);
	////���ʔ��ˌ�
	//float3 specular0 = pow(saturate(dot(reflect0, eyedir)), shininess) * specular;
	////���Z
	//float4 shadecolor;
	//shadecolor.rgb = (ambient0 + diffuse0 + specular0) * lightcolor;
	//shadecolor.w = 1.0f;

	//�A�e�ƃe�N�X�`���̐F������
	output.target0 = shadecolor * texcoord;
	output.target1 = float4(1,1,1,1);
	return output;
}