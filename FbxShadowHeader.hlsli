//ボーンの最大数
static const int MAX_BONES = 320;

cbuffer cbuff0 : register(b0)
{
	matrix view;	//ビュー行列
	matrix viewproj;	//ビュープロジェクション行列
	matrix world;	//ワールド行列
}

//cbuffer cbuff2 : register(b2)
//{
//	float3 lightv;	//ライトへの方向ベクトル
//	float3 lightcolor;	//ライトの色
//}

static const uint DIR_LIGHT_NUM = 3;

struct DirLight
{
	float3 lightv;	//ライトへの方向ベクトル
	float3 lightcolor;	//ライトの色
	uint active;
};

//点光源の数
static const int POINTLIGHT_NUM = 3;

struct PointLight
{
	float3 lightpos;	//ライト座標
	float3 lightcolor;	//ライトの色
	float3 lightatten;	//ライト距離減衰係数
	uint active;
};

//スポットライトの数
static const int SPOTLIGHT_NUM = 3;

struct SpotLight
{
	float3 lightv;		//ライトの光線方向の逆ベクトル
	float3 lightpos;	//ライト座標
	float3 lightcolor;	//ライトの色
	float3 lightatten;	//ライト距離減衰係数
	float2 lightfactoranglecos;	//ライト減衰角度のコサイン
	uint active;
};

//丸影の数
static const int SHADOW_NUM = 2;

struct Shadow
{
	uint active;
	matrix lightView;
	matrix lightViewProj;
};

//丸影の数
static const int CIRCLESHADOW_NUM = 2;

struct CircleShadow
{
	float3 dir;
	float3 casterPos;
	float distanceCasterLight;
	float3 atten;
	float2 factorAngleCos;
	uint active;
};

cbuffer cbuff2 : register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIR_LIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
	Shadow shadows[SHADOW_NUM];
	CircleShadow circleShadows[CIRCLESHADOW_NUM];
}

cbuffer skinning:register(b3)	//ボーンのスキニング行列が入る
{
	matrix matSkinning[MAX_BONES];
};

//バーテックスバッファの入力
struct VSInput
{
	float4 pos : POSITION;	//位置
	float3 normal : NORMAL;	//頂点法線
	float2 uv : TEXCOORD;	//テクスチャー座標
	uint4 boneIndices : BONEINDICES;	//ボーンの番号
	float4 boneWeights : BONEWEIGHTS;	//ボーンのスキンウェイト
};

//頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;	//システム用頂点座標
	float4 worldpos : POSITION;	//システム用頂点座標
	float3 normal : NORMAL;	//法線
	float2 uv : TEXCOORD;	//uv値

	float4 pos : POSITION2; // 射影変換座標
	float4 zCalcTex : TEX; // Z値算出用テクスチャ
	float4 col : COLOR0; // ディフューズ色
};

//cbuffer cbuff0 : register(b0)
//{
//	matrix viewproj;	//ビュープロジェクション行列
//	matrix world;	//ワールド行列
//	float3 cameraPos;	//カメラ座標
//}
//
//cbuffer cbuff2 : register(b2)
//{
//	float3 lightv;	//ライトへの方向ベクトル
//	float3 lightcolor;	//ライトの色
//}