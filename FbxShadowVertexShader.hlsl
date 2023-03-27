#include "FbxShadowHeader.hlsli"

//スキニング後の頂点法線が入る
struct SkinOutput
{
	float4 pos;
	float3 normal;
};

//スキニング計算
SkinOutput ComputeSkin(VSInput input)
{
	//ゼロクリア
	SkinOutput output = (SkinOutput)0;

	uint iBone;	//計算するボーン番号
	float weight;	//ボーンウェイト
	matrix m;	//スキニング行列	

	//ボーン0
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン1
	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン2
	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン3
	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	return output;
}

//エントリーポイント
VSOutput main(VSInput input)
{
	//スキニング計算
	SkinOutput skinned = ComputeSkin(input);
	//法線にワールド行列によるスケーリング 回転を適用
	float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));
	float4 wpos = mul(world, skinned.pos);
	//ピクセルシェーダに渡す値
	VSOutput output;
	//行列による座標返還
	output.svpos = mul(mul(viewproj, world), skinned.pos);
	//ワールド法線を次のステージに渡す
	output.normal = wnormal.xyz;
	//入力値をそのまま次のステージ渡す
	output.uv = input.uv;
	//ワールド座標
	output.worldpos = wpos;

	//影を作る場合
	if (shadows[0].active == true)
	{
		//カメラの目線によるワールドビュー変換
		float4x4 mat;
		mat = mul(world, view);
		mat = mul(mat, viewproj);
		output.pos = mul(input.pos, mat);

		//ライトの目線によるビュー変換

		mat = mul(world, shadows[0].lightView);
		mat = mul(mat, shadows[0].lightViewProj);
		output.zCalcTex = mul(input.pos, mat);

		//法線とライトの方向から頂点の色を決定
		float3 N = normalize(mul(input.normal, world));
		float3 LightDirect = normalize(float3(shadows[0].lightView._13,
			shadows[0].lightView._23, shadows[0].lightView._33));
		output.col = float4(0.0, 0.6, 1.0, 1.0) * (0.3 + dot(N, -LightDirect) * (1 - 0.3f));

	}

	return output;
}