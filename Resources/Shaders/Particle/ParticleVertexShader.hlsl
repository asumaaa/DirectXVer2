#include "ParticleHeader.hlsli"

//エントリーポイント
VSOutput main(float4 pos:POSITION)
{
	//ピクセルシェーダに渡す値
	VSOutput output;
	output.svpos = pos;

	return output;
}