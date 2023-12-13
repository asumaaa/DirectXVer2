#include "ThunderParticleHeader.hlsli"

//エントリーポイント
VSOutput main(VSInput input)
{
	//ピクセルシェーダに渡す値
    VSOutput output;
    output.startPos = input.startPos;
    output.endPos = input.endPos;
    output.scale = input.scale;
    output.splteVal = input.splteVal;
    output.frame = input.frame;
    output.seed = input.seed;

    return output;
}