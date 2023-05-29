#pragma once
#include "array"
#include "DirectXMath.h"
#include "vector"
#include "memory"
#include "string"
#include "list"

class JSONLoader
{
private:	//�G�C���A�X
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ObjectData {
		// �t�@�C����
		std::string fileName;
		// ���s�ړ�
		XMFLOAT3 position;
		// ��]�p
		XMFLOAT3 rotation;
		// �X�P�[�����O
		XMFLOAT3 scale;
	};

	//�����o�֐�
public:
	void LoadFile(const std::string fileName);

	//�Q�b�^�[
	ObjectData GetObjectData(int number) { return objectData[number]; }
	std::string GetFileName(int number) { return objectData[number].fileName; }
	XMFLOAT3 GetPosition(int number) { return objectData[number].position; }
	XMFLOAT3 GetRotation(int number) { return objectData[number].rotation; }
	XMFLOAT3 GetScale(int number){ return objectData[number].scale; }

	//�����o�ϐ�
private:
	std::vector<ObjectData>objectData;
};

