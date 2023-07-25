#include "ExplosionParticle1Header.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);
//エントリーポイント
float4 main(GSOutput input) : SV_TARGET
{
	//画像のデータ
	float4 texData = float4(tex.Sample(smp,input.uv));

	//RBG値を反転
	/*texData = float4(1.0f - texData.x, 1.0f - texData.y, 1.0f - texData.z, 1.0f);*/

	//アルファ値を合成
	//return float4(texData.x * 0.1, texData.y * 0.1, texData.z * 0.1, 1.0f);
	return float4(texData.x * 0.992, texData.y * 0.3f, 0.0f, 1.0f);
}