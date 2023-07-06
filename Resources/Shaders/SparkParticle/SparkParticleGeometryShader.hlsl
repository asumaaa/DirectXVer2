#include "SparkParticleHeader.hlsli"

//四角形の頂点数
static const uint vnum = 4;

//センターからオフセット
static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f,0.1f,0.0f),	//左下
	float4(-0.5f,+0.5f,0.1f,0.0f),	//左上
	float4(+0.5f,-0.5f,0.1f,0.0f),	//右下
	float4(+0.5f,+0.5f,0.1f,0.0f),	//右上
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

	/*float4 wpos = mul(mul(viewproj, world), input[0].pos);

	for (uint i = 0; i < vnum; i++)
	{
		float4 offset = offset_array[i] * input[0].scale;
		offset = mul(matBillboard, offset);
		element.svpos = input[0].pos + offset;
		element.svpos = mul(mat,element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}*/

	//for (uint i = 0; i < vnum; i++)
	//{
	//	//中心からのオフセットをスケーリング
	//	float4 offset = offset_array[i] * input[0].scale;
	//	offset = mul(matBillboard, offset);
	//	element.svpos = wpos + input[0].pos + offset;
	//	element.uv = uv_array[i];
	//	output.Append(element);
	//}

	//4点分まわす
	//for (uint i = 0; i < vnum; i++)
	//{
	//	//中心からのオフセットをビルボード回転
	//	//float4 offset = mul(matBillboard, offset_array[i]);
	//	//中心からのオフセットをスケーリング
	//	float4 offset = offset_array[i] * input[0].scale;
	//	//中心カランオフセットをビルボード回転
	//	offset = mul(matBillboard, offset);
	//	//オフセット分ずらす
	//	element.svpos = input[0].pos + offset;
	//	//ビュー、射影返還
	//	element.svpos = mul(mat, element.svpos);
	//	element.uv = uv_array[i];
	//	output.Append(element);
	//}

	//4点分まわす
	//for (uint i = 0; i < vnum; i++)
	//{
	//	//中心からのオフセットをビルボード回転
	//	float4 offset = mul(matBillboard, offset_array[i]);
	//	//オフセット分ずらす
	//	element.svpos = input[0].svpos + offset;
	//	//ビュー、射影返還
	//	element.svpos = mul(mat, element.svpos);
	//	element.uv = uv_array[i];
	//	output.Append(element);
	//}
}