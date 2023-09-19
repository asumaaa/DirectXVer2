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
	float4 texcolor = tex.Sample(smp, input.uv);
	
	//Camera
	float3 eyedir = normalize(cameraPos - input.worldpos);
	//Light
	float3 lightpos = float3(1, 1, -1);
	float3 lightdir = normalize(lightpos);
	float4 lightcolor = float4(color.x, color.y, color.z, 1);
	
	//Ambient
	float4 ambient = texcolor * 0.3 * lightcolor;
	ambient.w = 1;
	
	//diffuse
	float intensity = saturate(dot(normalize(input.normal), lightpos));
	float4 diffuse = texcolor * intensity * lightcolor;
	
	//specular
	float3 normal = normalize(input.normal);
	float3 reflectdir = -lightdir + 2 * normal * dot(normal, lightdir);
	float4 specular = pow(saturate(dot(reflectdir, eyedir)), 20);

	//abs
	float4 shadecolor = ambient + diffuse + specular;

	//�A�e�ƃe�N�X�`���̐F������
	output.target0 = shadecolor;
	output.target1 = float4(1,1,1,1);
	return output;
}