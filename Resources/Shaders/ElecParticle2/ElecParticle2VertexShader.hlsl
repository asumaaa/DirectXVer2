#include "ElecParticle2Header.hlsli"

//エントリーポイント
VSOutput main(float4 pos:POSITION, float scale:SCALE, float frame:FRAME)
{
	//ピクセルシェーダに渡す値
	VSOutput output;
	output.pos = pos;
	output.scale = scale;
    output.frame = frame;

	return output;
}