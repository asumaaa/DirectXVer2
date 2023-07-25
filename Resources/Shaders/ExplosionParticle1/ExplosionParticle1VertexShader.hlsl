#include "ExplosionParticle1Header.hlsli"

//エントリーポイント
VSOutput main(float4 pos:POSITION, float scale:SCALE,float4 velocity:VELOCITY)
{
	//ピクセルシェーダに渡す値
	VSOutput output;
	output.pos = pos;
	output.scale = scale;
	output.velocity = velocity;

	return output;
}