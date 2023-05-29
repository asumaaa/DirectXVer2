#include "JSONLoader.h"
#include "json.hpp"
#include "fstream"
#include "cassert"

#define PI 3.1415

void JSONLoader::LoadFile(const std::string fileName)
{
	//�I�u�W�F�N�g�f�[�^���Z�b�g
	objectData.clear();

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

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : jsonFileList["objects"]) {
		assert(object.contains("type"));

		// ��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0) {

			if (object.contains("file_name")) {
				// �t�@�C����
				objectData1.fileName = object["file_name"];
			}

			// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			// ���s�ړ�
			objectData1.position.x = (float)transform["translation"][1];
			objectData1.position.y = (float)transform["translation"][2];
			objectData1.position.z = -(float)transform["translation"][0];
			// ��]�p
			objectData1.rotation.x = (float)transform["rotation"][1];
			objectData1.rotation.y = (float)transform["rotation"][2];
			objectData1.rotation.z = (float)transform["rotation"][0];
			//�ʓx�@�ɕϊ�
			objectData1.rotation.x *= 1.0f / 360.0f * (2.0f * PI);
			objectData1.rotation.y *= 1.0f / 360.0f * (2.0f * PI);
			objectData1.rotation.z *= 1.0f / 360.0f * (2.0f * PI);
			// �X�P�[�����O
			objectData1.scale.x = (float)transform["scale"][1];
			objectData1.scale.y = (float)transform["scale"][2];
			objectData1.scale.z = (float)transform["scale"][0];

			// TODO: �R���C�_�[�̃p�����[�^�ǂݍ���

			// TODO: �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�ďo�Ŏ}�𑖍�����
			/*if (object.contains("children")) {

			}*/

			//�I�u�W�F�N�g�f�[�^�ɑ��
			objectData.push_back(objectData1);
		}
	}
}
