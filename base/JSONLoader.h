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

public:	//�T�u�N���X
	struct ObjectData {
		// �t�@�C����
		std::string fileName;
		//�I�u�W�F�N�g�̖��O
		std::string objectName;
		// ���s�ړ�
		DirectX::XMFLOAT3 position;
		// ��]�p
		DirectX::XMFLOAT3 rotation;
		// �X�P�[�����O
		DirectX::XMFLOAT3 scale;
	};

	struct ColliderData {
		// �R���C�_�[�̎��
		std::string type;
		//�I�u�W�F�N�g�̖��O
		std::string objectName;
		// �X�P�[�����O
		DirectX::XMFLOAT3 scale;
		// ��]�p
		DirectX::XMFLOAT3 rotation;
		//���S
		DirectX::XMFLOAT3 center;
	};

	//�����o�֐�
public:
	void LoadFile(const std::string fileName);

	//�Q�b�^�[
	ObjectData GetObjectData(int number) { return objectData[number]; }
	ColliderData GetColliderData(int number) { return colliderData[number]; }
	std::string GetFileName(int number) { return objectData[number].fileName; }
	XMFLOAT3 GetPosition(int number) { return objectData[number].position; }
	XMFLOAT3 GetRotation(int number) { return objectData[number].rotation; }
	XMFLOAT3 GetScale(int number){ return objectData[number].scale; }
	size_t GetObjectNum() { return objectNum; }

	//�����o�ϐ�
private:
	std::vector<ObjectData>objectData;
	std::vector<ColliderData>colliderData;
	size_t objectNum = 0.0f;
};
