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
	//�t�@�C���ǂݍ���
	std::stringstream posList;	//������
	std::vector<DirectX::XMFLOAT3>obstaclePos;
	//�t�@�C�����J��
	std::ifstream file;
	file.open("textureName");
	//�t�@�C���̓��e���R�s�[
	posList << file.rdbuf();
	//�t�@�C�������
	file.close();

	std::string line;

	//�t�@�C�������Q���̏ꏊ��ǂݍ���
	while (getline(posList, line, '/'))
	{
	}
}
