#include "PlayerBulletParticleHeader.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);
//エントリーポイント
float4 main(GSOutput input) : SV_TARGET
{
	//画像のデータ
	float4 texData = float4(tex.Sample(smp,input.uv));
	
    float f = 1.0f - input.frame;
    float r = 1.0f;
    float g = 0.3f;

	//アルファ値を合成
    return float4(texData.x * (r * f), texData.y * (g * f), 0.0f, 1);
}