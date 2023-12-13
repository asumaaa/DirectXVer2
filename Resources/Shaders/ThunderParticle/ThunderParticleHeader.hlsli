cbuffer cbuff0 : register(b0)
{
    matrix mat; //3D変換行列
    matrix matBillboard; //ワールド行列
}

//バーテックスバッファの入力
struct VSInput
{
    float4 startPos : START_POSITION; //システム用頂点座標
    float4 endPos : END_POSITION; //システム用頂点座標
    float scale : SCALE; //スケール
    float splteVal : SPLITE; //枝分かれ数
    float frame : FRAME; //フレーム
    float seed : SEED; //seed値
};

//頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 startPos : START_POSITION; //システム用頂点座標
    float4 endPos : END_POSITION; //システム用頂点座標
    float scale : SCALE; //スケール
    float splteVal : SPLITE; //枝分かれ数
    float frame : FRAME; //フレーム
    float seed : SEED; //seed値
};

//ジオメトリシェーダからピクセルシェーダへの出力
struct GSOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
    float frame : FRAME;
};