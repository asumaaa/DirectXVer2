#include "BillboardSpriteHeader.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);
//エントリーポイント
float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texcoord = tex.Sample(smp,input.uv);

	//テクスチャが透明な所の場合
	if (texcoord.w == 0)
	{
		discard;
	}

	return float4(texcoord.x, texcoord.y, texcoord.z,1);
}