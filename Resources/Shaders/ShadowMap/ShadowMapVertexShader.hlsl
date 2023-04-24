#include "ShadowMapHeader.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = mul(mat,pos);
	output.uv = uv;

	float4 pos0 = mul(pos, mat);
	pos0 = mul(pos0, lightvp);
	pos0.xyz = pos0.xyz / pos0.w;
	output.possm.x = (1.0f + pos0.x) / 2.0f;
	output.possm.y = (1.0f + pos0.y) / 2.0f;
	output.possm.z = pos0.z;

	return output;
}