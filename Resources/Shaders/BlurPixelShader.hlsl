#include "BlurEffectHeader.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float3 sampleBox(float2 uv,float resolution)
{
	return float3(0, 0, 0);
}

float4 main(VSOutput input) : SV_TARGET
{
	//Ç⁄Ç©Çµ
	int x = input.uv.x * window.x;
	int y = input.uv.y * window.y;
	float uvX = x/ window.x;
	float uvY = y/ window.y;
	float4 addColor = float4(0, 0, 0, 0);

	//ÉÇÉUÉCÉN
	/*int x = input.uv.x * window.x;
	int y = input.uv.y * window.y;
	x /= resolution;
	x *= resolution;
	y /= resolution;
	y *= resolution;
	float uvX = x / window.x;
	float uvY = y / window.y;
	float4 addColor = float4(0, 0, 0, 0);*/

	for (int i = 0; i < resolution; i++)
	{
		for (int j = 0; j < resolution; j++)
		{
			addColor += tex0.Sample(smp,float2(uvX + (i / window.x), uvY + (j / window.x))) /
				(resolution * resolution);
		}
	}

	return addColor;
}