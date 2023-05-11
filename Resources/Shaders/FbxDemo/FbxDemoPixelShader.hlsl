#include "FbxDemoHeader.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
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
	float4 texcoord = tex.Sample(smp,input.uv);
	//ライトの向き
	float3 lightdir = float3(1, -1, 1);
	//ライトの色
	float3 lightcolor = float3(1, 1, 1);
	//視点座標を固定
	float eyepos = float3(0, 10, 10);
	//Lambert反射
	float3 light = normalize(lightdir);	//右下奥 向きのライト
	float diffuse0 = saturate(dot(-light, input.normal));
	float brightness = diffuse0 + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	
	//Phong反射
	//環境反射光
	//float3 ambient0 = ambient;
	////拡散反射光
	//float3 diffuse0 = dot(-lightdir, input.normal) * diffuse;
	////光沢
	//const float shininess = 4.0f;
	////頂点から視点へのベクトル
	//const float3 eyedir = normalize(eyepos - input.pos.xyz);
	////反射光ベクトル
	//float3 reflect0 = normalize(lightdir + 2 * dot(-lightdir, input.normal) * input.normal);
	////鏡面反射光
	//float3 specular0 = pow(saturate(dot(reflect0, eyedir)), shininess) * specular;
	////加算
	//float4 shadecolor;
	//shadecolor.rgb = (ambient0 + diffuse0 + specular0) * lightcolor;
	//shadecolor.w = 1.0f;

	//陰影とテクスチャの色を合成
	output.target0 = shadecolor * texcoord;
	output.target1 = float4(1,1,1,1);
	return output;
}