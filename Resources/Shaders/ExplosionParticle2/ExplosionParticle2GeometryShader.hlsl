#include "ExplosionParticle2Header.hlsli"

//四角形の頂点数
static const uint vnum = 4;

//センターからオフセット
static const float4 offset_array[vnum] =
{
	float4(-2.5f,-2.5f,0.1f,0.0f),	//左下
	float4(-2.5f,+2.5f,0.1f,0.0f),	//左上
	float4(+2.5f,-2.5f,0.1f,0.0f),	//右下
	float4(+2.5f,+2.5f,0.1f,0.0f),	//右上
};

//uv
static const float2 uv_array[vnum] =
{
	float2(0.0f,1.0f),
	float2(0.0f,0.0f),
	float2(1.0f,1.0f),
	float2(1.0f,0.0f),
};

//点の入力から四角形を出力
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;

	for (uint i = 0; i < vnum; i++)
	{
		//中心からのオフセットをスケーリング
		float4 offset = offset_array[i] * input[0].scale;
		//中心からのオフセットをビルボード回転
		offset = mul(matBillboard, offset);
		//ワールド座標ベースでずらす
		element.svpos = input[0].pos + offset;
		//ビュー、射影変換
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}
}