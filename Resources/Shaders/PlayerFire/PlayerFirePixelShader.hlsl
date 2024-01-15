#include "PlayerFireHeader.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);	//テクスチャ1枚目
Texture2D<float> tex1 : register(t1);	//カメラからの深度
Texture2D<float> tex2 : register(t2);	//ライトからの深度
Texture2D<float4> tex3 : register(t3);	//テクスチャ2枚目
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
	
	//タイマーでUVをずらす
    input.uv = float2(input.uv.x + timer1, input.uv.y + timer1);
    if (input.uv.x >= 1.0f)
    {
        input.uv.x -= 1.0f;
    }
    if (input.uv.y >= 1.0f)
    {
        input.uv.y -= 1.0f;
    }
	//テクスチャマッピング
	//炎1
    float4 texcoord1 = tex.Sample(smp, input.uv);
	//炎2
    float4 texcoord2 = tex.Sample(smp, float2(input.uv.x + 0.25f, input.uv.y + 0.25f));
    float4 texcoord3 = tex.Sample(smp, float2(input.uv.x + 0.5f, input.uv.y + 0.5f));
    float4 texcoord4 = tex.Sample(smp, float2(input.uv.x + 0.75f, input.uv.y + 0.75f));
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
	
	//2枚の炎のテクスチャを合成
    float4 texcoord = texcoord1 + texcoord2 + texcoord3 + texcoord4;
	
    texcoord.x *= timer2 * 3.0f;
    texcoord.y *= timer2 * 3.0f;
    texcoord.z *= timer2 * 3.0f;
	
    if (texcoord.x <= 0.05)
    {
        discard;
    }
	//陰影とテクスチャの色を合成
    texcoord.x *= 0.5f;
	texcoord.y *= 0.5f;
	texcoord.z *= 0.5f;
	output.target0 = shadecolor * texcoord;
	//影を合成
	output.target0.x *= shadowWeight;
	output.target0.y *= shadowWeight;
	output.target0.z *= shadowWeight;
	output.target1 = float4(1,1,1,1);
	return output;
}