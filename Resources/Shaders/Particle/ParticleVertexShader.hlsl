#include "ParticleHeader.hlsli"

//エントリーポイント
VSOutput main(float4 pos:POSITION, float scale:SCALE)
{
	//ピクセルシェーダに渡す値
	VSOutput output;
	output.svpos = pos;
	output.scale = scale;

	return output;
}