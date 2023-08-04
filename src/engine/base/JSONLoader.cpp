#include "JSONLoader.h"
#include "json.hpp"
#include "fstream"
#include "cassert"
#include "stdio.h"

#define PI 3.1415

void JSONLoader::LoadFile(const std::string fileName)
{
	//�I�u�W�F�N�g�f�[�^���Z�b�g
	objectData.clear();
	colliderData.clear();
	textureData.clear();

	//�t�@�C�����J��
	std::ifstream file;
	file.open(fileName);
	assert(!file.fail());

	//json������
	nlohmann::json jsonFileList;
	//json������ɑ��
	file >> jsonFileList;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(jsonFileList.is_object());
	assert(jsonFileList.contains("name"));
	assert(jsonFileList["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name = jsonFileList["name"].get<std::string>();
	assert(name.compare("scene") == 0);

	//�I�u�W�F�N�g�f�[�^�i�[�p�C���X�^���X�𐶐�
	ObjectData objectData1;
	//�R���C�_�[�f�[�^�i�[�p�C���X�^���X�𐶐�
	ColliderData colliderData1;
	//�e�N�X�`���f�[�^�i�[�p�C���X�^���X�𐶐�
	TextureData textureData1;

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : jsonFileList["objects"]) {
		assert(object.contains("type"));

		// ��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0) {

			if (object.contains("name")) {
				// �I�u�W�F�N�g��
				objectData1.objectName = object["name"];
				colliderData1.objectName = object["name"];
			}

			if (object.contains("file_name")) {
				// �t�@�C����
				objectData1.fileName = object["file_name"];
			}

			// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			// ���s�ړ�
			objectData1.position.x = (float)transform["translation"][0];
			objectData1.position.y = (float)transform["translation"][2];
			objectData1.position.z = (float)transform["translation"][1];
			// ��]�p
			if (object["file_name"].get<std::string>() != "plane")
			{
				objectData1.rotation.x = (float)transform["rotation"][0] - 90;
				objectData1.rotation.y = (float)transform["rotation"][2] - 180;
				objectData1.rotation.z = -(float)transform["rotation"][1];
			}
			else
			{
				objectData1.rotation.x = (float)transform["rotation"][0];
				objectData1.rotation.y = (float)transform["rotation"][2];
				objectData1.rotation.z = -(float)transform["rotation"][1];
			}
			//�ʓx�@�ɕϊ�
			objectData1.rotation.x *= 1.0f / 360.0f * (2.0f * PI);
			objectData1.rotation.y *= 1.0f / 360.0f * (2.0f * PI);
			objectData1.rotation.z *= 1.0f / 360.0f * (2.0f * PI);
			// �X�P�[�����O
			objectData1.scale.x = (float)transform["scale"][0];
			objectData1.scale.y = (float)transform["scale"][2];
			objectData1.scale.z = (float)transform["scale"][1];

			// �R���C�_�[�̃p�����[�^�ǂݍ���
			nlohmann::json& collider = object["collider"];

			// �R���C�_�[�̎��
			colliderData1.type = collider["type"];
			//�R���C�_�[�̒��S
			colliderData1.center.x = (float)collider["center"][0];
			colliderData1.center.y = (float)collider["center"][2];
			colliderData1.center.z = (float)collider["center"][1];
			//�R���C�_�[�̃T�C�Y
			colliderData1.scale.x = (float)collider["size"][0];
			colliderData1.scale.y = (float)collider["size"][2];
			colliderData1.scale.z = -(float)collider["size"][1];
			//�R���C�_�[�̉�]�p
			colliderData1.rotation.x = (float)transform["rotation"][0];
			colliderData1.rotation.y = (float)transform["rotation"][2];
			colliderData1.rotation.z = -(float)transform["rotation"][1];
			/*if (object["file_name"].get<std::string>() != "plane")
			{
				colliderData1.rotation.x = (float)transform["rotation"][0] - 90;
				colliderData1.rotation.y = (float)transform["rotation"][2] - 180;
				colliderData1.rotation.z = -(float)transform["rotation"][1];
			}
			else
			{
				colliderData1.rotation.x = (float)transform["rotation"][0];
				colliderData1.rotation.y = (float)transform["rotation"][2];
				colliderData1.rotation.z = -(float)transform["rotation"][1];
			}*/
			//�ʓx�@�ɕϊ�
			colliderData1.rotation.x *= 1.0f / 360.0f * (2.0f * PI);
			colliderData1.rotation.y *= 1.0f / 360.0f * (2.0f * PI);
			colliderData1.rotation.z *= 1.0f / 360.0f * (2.0f * PI);

			//�e�N�X�`���f�[�^
			if (object.contains("textureNum1"))
			{
				textureData1.textureNum1 = stoi(object["textureNum1"].get<std::string>());
				textureData1.textureVol = 1;
			}
			if (object.contains("textureNum2"))
			{
				textureData1.textureNum2 = stoi(object["textureNum2"].get<std::string>());
				textureData1.textureVol = 2;
			}
			if (object.contains("textureNum3"))
			{
				textureData1.textureNum3 = stoi(object["textureNum3"].get<std::string>());
				textureData1.textureVol = 3;
			}
			if (object.contains("textureNum4"))
			{
				textureData1.textureNum4 = stoi(object["textureNum4"].get<std::string>());
				textureData1.textureVol = 4;
			}
			//�V�F�[�_��
			if (object.contains("shaderName"))
			{
				textureData1.shaderName = object["shaderName"].get<std::string>();
			}
			else
			{
				textureData1.shaderName = "null";
			}

			// TODO: �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�ďo�Ŏ}�𑖍�����
			/*if (object.contains("children")) {

			}*/

			//�I�u�W�F�N�g�f�[�^�ɑ��
			objectData.push_back(objectData1);
			//�R���C�_�[�f�[�^�ɑ��
			colliderData.push_back(colliderData1);
			//�e�N�X�`���f�[�^�ɑ��
			textureData.push_back(textureData1);

			//�I�u�W�F�N�g�̐��𑝂₷
			objectNum++;
		}
	}
}
