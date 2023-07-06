cbuffer cbuff0 : register(b0)
{
	matrix mat;	//3D変換行列
	matrix matBillboard;	//ビルボード行列
	matrix world;	//ワールド行列
	matrix viewproj;	//射影変換
}

//頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : POSITION;	//システム用頂点座標
	float scale : SCALE;	//スケール
	float4 wpos:TEXCOORD;
};

//ジオメトリシェーダからピクセルシェーダへの出力
struct GSOutput
{
	float4 svpos:SV_POSITION;	//システム用頂点座標
	float2 uv:TEXCOORD;	//uv値
};