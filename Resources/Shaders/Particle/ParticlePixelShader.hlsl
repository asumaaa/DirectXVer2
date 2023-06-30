#include "ParticleHeader.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);
//エントリーポイント
float4 main(VSOutput input) : SV_TARGET
{
	//画像のデータ
	float4 texData = float4(tex.Sample(smp,input.uv));

	//アルファ値を合成
	return float4(texData.x * color.x, texData.y * color.y, texData.z * color.z, 1);
}