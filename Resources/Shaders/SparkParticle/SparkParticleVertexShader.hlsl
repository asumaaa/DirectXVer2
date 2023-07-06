#include "SparkParticleHeader.hlsli"

//エントリーポイント
VSOutput main(float4 pos:POSITION, float scale:SCALE)
{
	//ピクセルシェーダに渡す値
	VSOutput output;
	output.pos = pos;
	output.scale = scale;

	return output;
}