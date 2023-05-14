#include "FogHeader.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float> tex1 : register(t1);	//カメラからの深度
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{			
	float3 fogColor = 0.8f;
	float fogWeight = 0.0f;

	float4 colortex0 = tex0.Sample(smp,input.uv);
	float dep0 = pow(tex1.Sample(smp, input.uv), 300);

	if (dep0 > startDepth)
	{
		float c = (dep0 - startDepth) * strength;

		colortex0.x += c;
		colortex0.y += c;
		colortex0.z += c;
	}
	//アルファ値合成
	colortex0.x *= 1.0f;
	colortex0.y *= 1.0f;
	colortex0.z *= 0.0f;

	colortex0.w *= color.w;

	/*return float4(dep0, dep0, dep0, 1);*/
	
	return colortex0;
}