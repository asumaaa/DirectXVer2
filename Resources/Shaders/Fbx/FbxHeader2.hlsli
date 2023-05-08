//ボーンの最大数
static const int MAX_BONES = 32;

cbuffer cbuff0 : register(b0)
{
	matrix viewproj;	//ビュープロジェクション行列
	matrix world;	//ワールド行列
	float3 cameraPos;	//カメラ座標
	matrix lightviewproj;	//ビュープロジェクション行列
	matrix shadow;
};

cbuffer skinning:register(b3)	//ボーンのスキニング行列が入る
{
	matrix matSkinning[MAX_BONES];
};

static const uint dirLightNum = 3;
struct DirectionalLight
{
	float3 lightv;
	float3 lightcolor;
	uint active;
};

static const uint pointLightNum = 3;
struct PointLight
{
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	uint active;
};

static const uint spotLightNum = 1;
struct SpotLight
{
	float3 lightv;
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	float2 lightfactoranglecos;
	uint active;
};

static const uint circleShadowNum = 1;
struct CircleShadow
{
	float3 dir;
	float3 casterPos;
	float3 atten;
	float2 factorAngleCos;
	uint active;
};

cbuffer cbuff4 : register(b4)
{
	float3 ambientColor;
	DirectionalLight dirLights[dirLightNum];
	PointLight pointLights[pointLightNum];
	SpotLight spotLights[spotLightNum];
	CircleShadow circleShadows[circleShadowNum];
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
	float4 tpos : TPOS;
};