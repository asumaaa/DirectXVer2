#include "BlurEffectHeader.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

//ガウス関数
float Gaussian(float2 inputUV, float2 pickUV,float sigma)
{
	float dis = distance(inputUV, pickUV);
	return exp(-(dis * dis) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	float x = input.uv.x * window.x;
	float y = input.uv.y * window.y;
	float uvX = x/ window.x;
	float uvY = y/ window.y;
	float totalWeight = 0.0f;
	float4 color = float4(0,0,0,0);

	for (int i = 0; i < strengthWeight.x; i++)
	{
		for (int j = 0; j < strengthWeight.y; j++)
		{
			float2 pickUV = float2(uvX + (i / window.x), uvY + (j / window.y));
			float4 colortex0 = tex0.Sample(smp, pickUV);
			float weight = Gaussian(input.uv, pickUV, 0.05f);	//重み取得
			if (colortex0.w != 0)color += tex0.Sample(smp, pickUV) * weight;	//Gaussianで取得した重みに色をかける
			totalWeight += weight;						//かけた重み分の合計値を控えておく
		}
	}

	color = float4(color.x / totalWeight, color.y / totalWeight, color.z / totalWeight, 1.0f);
	return color;

	/*addColor = tex0.Sample(smp, input.uv);

	return addColor;*/
}