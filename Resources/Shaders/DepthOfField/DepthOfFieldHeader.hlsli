//マテリアル
cbuffer ConstBuff : register(b0)
{
	float4 color;

	/*float3 status;*/
	////フォーカス
	//float focus;
	////F値
	//float fnumber;
	////強さ
	//float strength;

	float2 window;
};

//変形行列
cbuffer ConstBuffTransform : register(b1)
{
	matrix mat;
};

//フォーカスとか
cbuffer ConstBuffStatus : register(b2)
{
	//フォーカス
	float focus;
	//f値
	float fnumber;
	//強さ
	float strength;
};

//頂点シェーダの出力構造体
struct VSOutput
{
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	//uv
	float2 uv : TEXCOORD;
};