#include "ChromaticAberrationEffectHeader.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float2 uv0 = input.uv;
	float4 colortex0 = tex0.Sample(smp, uv0);
	uv0.x += strength;
	colortex0.r = tex0.Sample(smp, uv0).r;

	colortex0.w *= color.w;

	return colortex0;
}