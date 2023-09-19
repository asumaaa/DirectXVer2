#include "VignetteHeader.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp,input.uv);
	//float vignette = length(float2(0.5, 0.5) - input.uv);	//float2(0.5,0.5)ビネットの中心座標を設定
	//vignette = clamp(vignette - 0.5, 0, 1);
	//colortex0.rgb -= vignette;

	float2 vp = (input.uv);
	colortex0.rgb -= dot(vp, vp) * strength;

	return colortex0;
}