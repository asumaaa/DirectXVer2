#include "ThunderParticleHeader.hlsli"

//最大出力超点数
static const uint vnum = 204;
//1つの枝分を描画するのに必要な頂点
static const uint vnum2 = 8;

//センターからオフセット
static const float4 offset_array[4] =
{
    float4(-0.5f, -0.5f, 0.0f, 0.0f), //左下
	float4(-0.5f, +0.5f, 0.0f, 0.0f), //左上
	float4(+0.5f, -0.5f, 0.0f, 0.0f), //右下
	float4(+0.5f, +0.5f, 0.0f, 0.0f), //右上
};

//ランダムな値を得る関数 0.0f ~ 1.0f
float GetRand(float3 pos, int Seed)
{
    return frac(sin(dot(pos.xy * 7.931, float2(12.9898, 78.233)) + Seed) * 43758.5453);
}

//点の入力から四角形を出力
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
    
    //全体の雷を描くのに必要な頂点数
    float vnum3 = vnum2 * input[0].splteVal;
    
    //頂点データ
    float4 vPos[10];
    
    //頂点データの設定
    for (uint i = 0; i < input[0].splteVal; i++)
    {
        //一つ目の座標を最初の座標に
        if (i == 0)
        {
            vPos[0] = input[0].startPos;
            continue;
        }
        //最後の座標を目標の座標に
        if (i == input[0].splteVal - 1)
        {
            vPos[input[0].splteVal - 1] = input[0].endPos;
            continue;
        }

        //残りのベクトルを計算
        float3 vec = input[0].endPos - vPos[i - 1];
        //分割数分で割る
        vec = vec / (input[0].splteVal - i);
        //乱数を生成 0.5 ~ 1.0
        float xRand, yRand, zRand;
        xRand = GetRand(vPos[i - 1], input[0].seed * 100 + i) + 0.5;
        yRand = GetRand(vPos[i - 1], input[0].seed * 100 + i + 10) + 0.5;
        zRand = GetRand(vPos[i - 1], input[0].seed * 100 + i + 20) + 0.5;
        //乱数と分割されたベクトルをかける
        float vecX = vec.x * xRand;
        float vecY = vec.y * yRand;
        float vecZ = vec.z * zRand;
        vPos[i] = float4(vecX, vecY, vecZ,1.0f);
    }
	
	//現在の頂点
    float4 vertexPos[vnum2];

    for (uint i = 0; i < vnum3; i++)
    {
        if (i == 0)
        {
            vertexPos[0] = (vPos[0] + offset_array[0] * input[0].scale);
            vertexPos[1] = (vPos[1] + offset_array[0] * input[0].scale);
            vertexPos[2] = (vPos[0] + offset_array[1] * input[0].scale);
            vertexPos[3] = (vPos[1] + offset_array[1] * input[0].scale);
            vertexPos[4] = (vPos[0] + offset_array[2] * input[0].scale);
            vertexPos[5] = (vPos[1] + offset_array[2] * input[0].scale);
            vertexPos[6] = (vPos[0] + offset_array[3] * input[0].scale);
            vertexPos[7] = (vPos[1] + offset_array[3] * input[0].scale);
        }
        else if (i % 8 == 0)
        {
            vertexPos[0] = (vPos[i / 8] + offset_array[0] * input[0].scale);
            vertexPos[1] = (vPos[i / 8 + 1] + offset_array[0] * input[0].scale);
            vertexPos[2] = (vPos[i / 8] + offset_array[1] * input[0].scale);
            vertexPos[3] = (vPos[i / 8 + 1] + offset_array[1] * input[0].scale);
            vertexPos[4] = (vPos[i / 8] + offset_array[2] * input[0].scale);
            vertexPos[5] = (vPos[i / 8 + 1] + offset_array[2] * input[0].scale);
            vertexPos[6] = (vPos[i / 8] + offset_array[3] * input[0].scale);
            vertexPos[7] = (vPos[i / 8 + 1] + offset_array[3] * input[0].scale);
        }
        
		//中心からのオフセットをスケーリング
        float4 offset = vertexPos[i] * input[0].scale;
		//中心からのオフセットをビルボード回転
        offset = mul(matBillboard, offset);
		//ワールド座標ベースでずらす
        element.svpos = vertexPos[i] * input[0].scale;
		//ビュー、射影変換
        element.svpos = mul(mat, element.svpos);
        element.frame = input[0].frame;
        output.Append(element);
    }
}