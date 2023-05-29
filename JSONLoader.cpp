#include "JSONLoader.h"
#include "json.hpp"
#include "fstream"
#include "cassert"

#define PI 3.1415

void JSONLoader::LoadFile(const std::string fileName)
{
	//オブジェクトデータリセット
	objectData.clear();

	//ファイルを開く
	std::ifstream file;
	file.open(fileName);
	assert(!file.fail());

	//json文字列
	nlohmann::json jsonFileList;
	//json文字列に代入
	file >> jsonFileList;

	//正しいレベルデータファイルかチェック
	assert(jsonFileList.is_object());
	assert(jsonFileList.contains("name"));
	assert(jsonFileList["name"].is_string());

	//"name"を文字列として取得
	std::string name = jsonFileList["name"].get<std::string>();
	assert(name.compare("scene") == 0);

	//オブジェクトデータ格納用インスタンスを生成
	ObjectData objectData1;

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : jsonFileList["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0) {

			if (object.contains("file_name")) {
				// ファイル名
				objectData1.fileName = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData1.position.x = (float)transform["translation"][1];
			objectData1.position.y = (float)transform["translation"][2];
			objectData1.position.z = -(float)transform["translation"][0];
			// 回転角
			objectData1.rotation.x = (float)transform["rotation"][1];
			objectData1.rotation.y = (float)transform["rotation"][2];
			objectData1.rotation.z = (float)transform["rotation"][0];
			//弧度法に変換
			objectData1.rotation.x *= 1.0f / 360.0f * (2.0f * PI);
			objectData1.rotation.y *= 1.0f / 360.0f * (2.0f * PI);
			objectData1.rotation.z *= 1.0f / 360.0f * (2.0f * PI);
			// スケーリング
			objectData1.scale.x = (float)transform["scale"][1];
			objectData1.scale.y = (float)transform["scale"][2];
			objectData1.scale.z = (float)transform["scale"][0];

			// TODO: コライダーのパラメータ読み込み

			// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
			/*if (object.contains("children")) {

			}*/

			//オブジェクトデータに代入
			objectData.push_back(objectData1);
		}
	}
}
