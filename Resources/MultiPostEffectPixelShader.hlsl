#include "MultiPostEffect.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	////画像のデータ
	//float4 texData = float4(tex0.Sample(smp,input.uv));

	////アルファ値を合成
	//return float4(texData.x * color.x, texData.y * color.y, texData.z * color.z, texData.w * color.w);

	float4 colortex0 = tex0.Sample(smp,input.uv);
	float4 colortex1 = tex1.Sample(smp,input.uv);

	//反転処理
	float4 color0;
	color0.x = 1 - colortex0.x;
	color0.y = 1 - colortex0.y;
	color0.z = 1 - colortex0.z;
	if (colortex0.x != 0)
	{
		color0.w = colortex0.w;
	}
	else
	{
		color0.w = 1;
	}

	//ぼかし処理
	float resolution = 10;
	float windowx = 720;
	float windowy = 1280;
	int x = input.uv.x * windowx;
	int y = input.uv.y * windowy;
	x /= resolution;
	x *= resolution;
	y /= resolution;
	y *= resolution;
	float uvX = x / windowx;
	float uvY = y / windowy;
	float4 addColor = float4(0, 0, 0, 0);

	for (int i = 0; i < resolution; i++)
	{
		for (int j = 0; j < resolution; j++)
		{
			addColor += tex1.Sample(smp, float2(uvX + (i / windowx), uvY + (j / windowx))) /
				(resolution * resolution);
		}
	}

	addColor.w *= 0.3;

	float4 color = color0;
	if (fmod(input.uv.y, 0.1f) < 0.05f) {
		color = addColor;
	}

	return color;
}