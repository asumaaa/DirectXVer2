#include "ReversalEffectHeader.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp,input.uv);

	float4 color;
	color.x = 1 - colortex0.x;
	color.y = 1 - colortex0.y;
	color.z = 1 - colortex0.z;
	if (colortex0.x != 0)
	{
		color.w = colortex0.w;
	}
	else
	{
		color.w = 1;
	}

	return color;
}