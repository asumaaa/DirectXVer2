#include "NoizTestHeader.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);	//�e�N�X�`��1����
texture2D<float> tex1 : register(t1);	//�J��������̐[�x
texture2D<float> tex2 : register(t2);	//���C�g����̐[�x
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

float4 ran(float2 uv) : SV_TARGET
{
	//�e��
	/*float density = 10.0f;
	float2 u = floor(uv * density) / density;*/
	float randam = frac(sin(dot(uv, float2(12.9898, 78.233))) * 42758.5453 * timer);
	return float4(randam, randam, randam, 1.0f);
}

float2 randomVec(float2 fact)
{
	float2 angle = float2(
		dot(fact, float2(127.1f, 311.7f)), dot(fact, float2(269.5f, 183.3f)));

	return frac(sin(angle) * 45758.5453123 * timer) * 2 - 1;
}

float ParlinNoise(float density, float2 uv) : SV_TARGET
{
	float2 uvFloor = floor(uv * density);
	float2 uvFrac = frac(uv * density);

	float2 v00 = randomVec(uvFloor + float2(0, 0));
	float2 v01 = randomVec(uvFloor + float2(0, 1));
	float2 v10 = randomVec(uvFloor + float2(1, 0));
	float2 v11 = randomVec(uvFloor + float2(1, 1));

	float c00 = dot(v00, uvFrac - float2(0, 0));
	float c01 = dot(v01, uvFrac - float2(0, 1));
	float c10 = dot(v10, uvFrac - float2(1, 0));
	float c11 = dot(v11, uvFrac - float2(1, 1));

	float2 u = uvFrac * uvFrac * (3 - 2 * uvFrac);

	float v0010 = lerp(c00, c10, u.x);
	float v0111 = lerp(c01, c11, u.x);

	return lerp(v0010, v0111, u.y) / 2 + 0.5;
}

float4 frag(float2 uv) : SV_TARGET
{
	//�m�C�Y�쐬
	float density = 10.0f;

	/*float v00 = ran((floor(uv * density)) / density);
	float v01 = ran((floor(uv * density) + float2(0,1)) / density);
	float v10 = ran((floor(uv * density) + float2(1,0)) / density);
	float v11 = ran((floor(uv * density) + float2(1,1)) / density);

	float2 p = frac(uv * density);
	float2 v = p * p * (3 - 2 * p);
	float v0010 = lerp(v00, v10, v.x);
	float v0111 = lerp(v01, v11, v.x);
	float r = lerp(v0010, v0111, p.y);

	return float4(r, r, r, 1.0f);*/

	float pn = ParlinNoise(density, uv);
	return float4(pn, pn, pn, 1.0f);
}

PSOutput main(VSOutput input) : SV_TARGET
{
	PSOutput output;
	//�e�N�X�`���}�b�s���O
	//���̉摜
	//float4 texcoord1 = tex.Sample(smp, input.uv);
	//Lambert����
	float3 light = normalize(float3(1,-1,1));	//�E���� �����̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//���C�g���猩��UV�l
	float3 posFromLightVP = input.tpos.xyz / input.tpos.w;
	float2 shadowUV = (posFromLightVP + float2(1, -1)) * float2(0.5, -0.5);
	//�e�e�N�X�`��
	float depthFromLight = tex2.Sample(smp, shadowUV);
	float shadowWeight = 1.0f;
	if (depthFromLight < posFromLightVP.z - 0.005)
	{
		shadowWeight = 0.2f;
	}
	//�A�e�ƃe�N�X�`���̐F������
	//texcoord1.x *= 0.5f;
	//texcoord1.y *= 0.5f;
	//texcoord1.z *= 0.5f;
	//texcoord3 = float4(1, 0, 0, 1);
	//output.target0 = shadecolor * texcoord1;

	//�m�C�Y�쐬
	float density = 10.0f;
	float v00 = ran(floor(input.uv * density));
	float v01 = ran(floor(input.uv * density) + float2(0,1));
	float v10 = ran(floor(input.uv * density) + float2(1,0));
	float v11 = ran(floor(input.uv * density) + float2(1,1));

	float2 p = frac(input.uv * density);
	float v0010 = lerp(v00, v10, p.x);
	float v0111 = lerp(v01, v11, p.x);
	float4 r = frag(input.uv);

	output.target0 = r;
	//�e������
	output.target0.x *= shadowWeight;
	output.target0.y *= shadowWeight;
	output.target0.z *= shadowWeight;
	output.target1 = float4(1,1,1,1);
	return output;
}