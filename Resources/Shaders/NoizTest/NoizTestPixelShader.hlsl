#include "NoizTestHeader.hlsli"

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

float4 ran(float2 uv) : SV_TARGET
{
	//粗さ
	/*float density = 10.0f;
	float2 u = floor(uv * density) / density;*/
	float randam = frac(sin(dot(uv, float2(12.9898, 78.233))) * 42758.5453 * timer);
	return float4(randam, randam, randam, 1.0f);
}

float2 randomVec(float2 fact)
{
	float2 angle = float2(
		dot(fact, float2(127.1f, 311.7f)), dot(fact, float2(269.5f, 183.3f)));

	return frac(sin(angle) * 45758.5453123 * timer) * 2 - 1;
}

float ParlinNoise(float density, float2 uv) : SV_TARGET
{
	float2 uvFloor = floor(uv * density);
	float2 uvFrac = frac(uv * density);

	float2 v00 = randomVec(uvFloor + float2(0, 0));
	float2 v01 = randomVec(uvFloor + float2(0, 1));
	float2 v10 = randomVec(uvFloor + float2(1, 0));
	float2 v11 = randomVec(uvFloor + float2(1, 1));

	float c00 = dot(v00, uvFrac - float2(0, 0));
	float c01 = dot(v01, uvFrac - float2(0, 1));
	float c10 = dot(v10, uvFrac - float2(1, 0));
	float c11 = dot(v11, uvFrac - float2(1, 1));

	float2 u = uvFrac * uvFrac * (3 - 2 * uvFrac);

	float v0010 = lerp(c00, c10, u.x);
	float v0111 = lerp(c01, c11, u.x);

	return lerp(v0010, v0111, u.y) / 2 + 0.5;
}

float4 frag(float2 uv) : SV_TARGET
{
	//ノイズ作成
	float density = 10.0f;

	/*float v00 = ran((floor(uv * density)) / density);
	float v01 = ran((floor(uv * density) + float2(0,1)) / density);
	float v10 = ran((floor(uv * density) + float2(1,0)) / density);
	float v11 = ran((floor(uv * density) + float2(1,1)) / density);

	float2 p = frac(uv * density);
	float2 v = p * p * (3 - 2 * p);
	float v0010 = lerp(v00, v10, v.x);
	float v0111 = lerp(v01, v11, v.x);
	float r = lerp(v0010, v0111, p.y);

	return float4(r, r, r, 1.0f);*/

	float pn = ParlinNoise(density, uv);
	return float4(pn, pn, pn, 1.0f);
}

PSOutput main(VSOutput input) : SV_TARGET
{
	PSOutput output;
	//テクスチャマッピング
	//元の画像
	//float4 texcoord1 = tex.Sample(smp, input.uv);
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
	//texcoord1.x *= 0.5f;
	//texcoord1.y *= 0.5f;
	//texcoord1.z *= 0.5f;
	//texcoord3 = float4(1, 0, 0, 1);
	//output.target0 = shadecolor * texcoord1;

	//ノイズ作成
	float density = 10.0f;
	float v00 = ran(floor(input.uv * density));
	float v01 = ran(floor(input.uv * density) + float2(0,1));
	float v10 = ran(floor(input.uv * density) + float2(1,0));
	float v11 = ran(floor(input.uv * density) + float2(1,1));

	float2 p = frac(input.uv * density);
	float v0010 = lerp(v00, v10, p.x);
	float v0111 = lerp(v01, v11, p.x);
	float4 r = frag(input.uv);

	output.target0 = r;
	//影を合成
	output.target0.x *= shadowWeight;
	output.target0.y *= shadowWeight;
	output.target0.z *= shadowWeight;
	output.target1 = float4(1,1,1,1);
	return output;
}