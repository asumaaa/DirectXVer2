#include "DepthOfFieldHeader.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp, input.uv);
	float dep0 = pow(tex1.Sample(smp, input.uv), 300);

	int x = input.uv.x * window.x;
	int y = input.uv.y * window.y;
	float uvX = x / window.x;
	float uvY = y / window.y;
	float4 addColor = float4(0, 0, 0, 0);

	for (int i = 0; i < strength; i++)
	{
		for (int j = 0; j < strength; j++)
		{
			addColor += tex0.Sample(smp, float2(uvX + (i / window.x), uvY + (j / window.x))) /
				(strength * strength);
		}
	}

	addColor.w *= color.w;

	//[“x’l‚ªŽw’èˆÈ“à‚¾‚Á‚½‚ç‚Ú‚©‚·
	if (dep0 > focus + fnumber || dep0 < focus - fnumber)
	{
		return addColor;
	}

	/*return float4(dep0, dep0, dep0, 1);*/
	return colortex0;
}