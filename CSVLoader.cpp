#include "CSVLoader.h"
#include<fstream>
#include "sstream"
#include "stdio.h"

void CSVLoader::SetObjectNum(float num)
{
	objectNum = num;
	position.resize(objectNum);	
	rotation.resize(objectNum);
	scale.resize(objectNum);
}

void CSVLoader::LoadCSV(const std::string textureName)
{
	//ファイル読み込み
	std::stringstream posList;	//文字列
	std::vector<DirectX::XMFLOAT3>obstaclePos;
	//ファイルを開く
	std::ifstream file;
	file.open("textureName");
	//ファイルの内容をコピー
	posList << file.rdbuf();
	//ファイルを閉じる
	file.close();

	std::string line;

	//ファイルから障害物の場所を読み込み
	while (getline(posList, line, '/'))
	{
	}
}
