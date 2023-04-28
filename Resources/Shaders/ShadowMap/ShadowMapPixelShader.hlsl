#include "ShadowMapHeader.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float> tex1 : register(t1);	//カメラからの深度
texture2D<float> tex2 : register(t2);	//ライトからの深度
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	/*float shadow = tex1.Sample(smp,input.possm.xy);
	float shadowA = (input.possm.z - 0.005f < shadow) ? 1.0f : 0.5f;	*/				

	float4 colortex0 = tex0.Sample(smp,input.uv)/* * shadowA*/;
	float dep0 = pow(tex1.Sample(smp, input.uv), 20);
	float dep1 = pow(tex2.Sample(smp, input.uv), 20);

	/*return float4(dep1, dep1, dep1, 1);*/

	
	return colortex0;
	return colortex0;
}