#include "SpriteHeader.hlsli"

texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texData = float4(tex.Sample(smp,input.uv));

	return float4(texData.x * color.x, texData.y * color.y, texData.z * color.z, texData.w * color.w);
}