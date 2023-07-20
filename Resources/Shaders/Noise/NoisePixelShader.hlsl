#include "NoiseHeader.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);	//テクスチャ1枚目
texture2D<float> tex1 : register(t1);	//カメラからの深度
texture2D<float> tex2 : register(t2);	//ライトからの深度
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input) : SV_TARGET
{
	PSOutput output;
	//テクスチャマッピング
	//元の画像
	float4 texcoord1 = tex.Sample(smp, input.uv);
	//Lambert反射
	float3 light = normalize(float3(1,-1,1));	//右下奥 向きのライト
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//ライトから見たUV値
	float3 posFromLightVP = input.tpos.xyz / input.tpos.w;
	float2 shadowUV = (posFromLightVP + float2(1, -1)) * float2(0.5, -0.5);
	//影テクスチャ
	float depthFromLight = tex2.Sample(smp, shadowUV);
	float shadowWeight = 1.0f;
	if (depthFromLight < posFromLightVP.z - 0.005)
	{
		shadowWeight = 0.2f;
	}
	//陰影とテクスチャの色を合成
	texcoord1.x *= 0.5f;
	texcoord1.y *= 0.5f;
	texcoord1.z *= 0.5f;
	//texcoord3 = float4(1, 0, 0, 1);
	//output.target0 = shadecolor * texcoord1;
	float randam = frac(sin(dot(input.uv, float2(12.9898, 78.233))) * 42758.5453 * timer);
	output.target0 = float4(randam, randam, randam, 1.0f);
	//影を合成
	output.target0.x *= shadowWeight;
	output.target0.y *= shadowWeight;
	output.target0.z *= shadowWeight;
	output.target1 = float4(1,1,1,1);
	return output;
}