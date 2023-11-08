#include "ThunderParticleHeader.hlsli"

//エントリーポイント
VSOutput main(float4 pos : POSITION, float4 prePos : SV_POSITION, float scale: SCALE)
{
	//ピクセルシェーダに渡す値
    VSOutput output;
    output.pos = pos;
    output.prePos = prePos;
    output.scale = scale;

    return output;
}