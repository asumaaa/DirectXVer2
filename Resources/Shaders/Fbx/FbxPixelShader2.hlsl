#include "FbxHeader2.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
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
	float4 texcoord = tex.Sample(smp,input.uv);
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


	////頂点から視点へのベクトル
	//float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	////光沢
	//float3 shininess = 4.0f;

	////点光源
	//for (int i = 0; i < pointLightNum; i++)
	//{
	//	if (pointLights[i].active)
	//	{
	//		//ライトのベクトル
	//		float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
	//		//ベクトルの長さ
	//		float d = length(lightv);
	//		//正規化し、単位ベクトルにする
	//		lightv = normalize(lightv);
	//		//距離減衰係数
	//		float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d +
	//			pointLights[i].lightatten.z * d * d);
	//		//ライトに向かうベクトルと法線の内積
	//		float3 dotlightnormal = dot(lightv, input.normal);
	//		//反射光ベクトル
	//		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//		//拡散反射光
	//		//float3 diffuse = dotlightnormal * m_diffuse;
	//		float diffuse1 = saturate(dot(-light, input.normal));
	//		//鏡面反射光
	//		/*float3 specular = pow(saturate(dot(reflect,eyedir)),shininess)*/
	//		shadecolor.rgb *= atten/* * (diffuse1)*pointLights[i].lightcolor*/;
	//	}
	//}

	//for (int i = 0; i < spotLightNum; i++)
	//{
	//	if (spotLights[i].active)
	//	{
	//		//ライトへの方向ベクトル
	//		float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
	//		//ベクトルの長さ
	//		float d = length(lightv);
	//		//正規化し、単位ベクトルにする
	//		lightv = normalize(lightv);
	//		//距離減衰係数
	//		float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
	//		// 角度減衰
	//		float cos = dot(lightv, spotLights[i].lightv);
	//		//減衰開始角度から、 減衰終了角度にかけて減衰
	//		// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
	//		float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
	//		// 角度減衰を乗算
	//		atten *= angleatten;
	//		//ライトに向かうベクトルと法線の内積
	//		float3 dotlightnormal = dot(lightv, input.normal);
	//		//反射光ベクトル
	//		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//		//拡散反射光
	//		float diffuse1 = saturate(dot(-light, input.normal));
	//		// 鏡面反射光
	//		/*float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;*/
	//		//全て加算する
	//		shadecolor.rgb *= atten/* * (diffuse + specular) * spotLights[i].lightcolor*/;
	//	}
	//}
	
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