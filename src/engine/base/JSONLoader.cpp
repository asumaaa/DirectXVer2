/**
 * @file JSONLoader.cpp
 * @brief JSONファイルをゲームで使う形で読み込む
 * @author Asuma Syota
 * @date 2023/4
 */

#include "JSONLoader.h"
#include "json.hpp"
#include "fstream"
#include "cassert"
#include "stdio.h"

#define PI 3.1415

void JSONLoader::LoadFile(const std::string fileName)
{
	//オブジェクトデータリセット
	objectData.clear();
	colliderData.clear();
	textureData.clear();

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
	//コライダーデータ格納用インスタンスを生成
	ColliderData colliderData1;
	//テクスチャデータ格納用インスタンスを生成
	TextureData textureData1;

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : jsonFileList["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0) {

			if (object.contains("name")) {
				// オブジェクト名
				objectData1.objectName = object["name"];
				colliderData1.objectName = object["name"];
			}

			if (object.contains("file_name")) {
				// ファイル名
				objectData1.fileName = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData1.position.x = (float)transform["translation"][0];
			objectData1.position.y = (float)transform["translation"][2];
			objectData1.position.z = (float)transform["translation"][1];
			// 回転角
			if (object["file_name"].get<std::string>() != "plane")
			{
				objectData1.rotation.x = (float)transform["rotation"][0] - 90;
				objectData1.rotation.y = (float)transform["rotation"][2];
				objectData1.rotation.z = (float)transform["rotation"][1];
			}
			else
			{
				objectData1.rotation.x = (float)transform["rotation"][0] - 90;
				objectData1.rotation.y = (float)transform["rotation"][2];
				objectData1.rotation.z = -(float)transform["rotation"][1];
			}
			//弧度法に変換
			objectData1.rotation.x *= 1.0f / 360.0f * (2.0f * PI);
			objectData1.rotation.y *= 1.0f / 360.0f * (2.0f * PI);
			objectData1.rotation.z *= 1.0f / 360.0f * (2.0f * PI);
			// スケーリング
			objectData1.scale.x = (float)transform["scale"][0];
			objectData1.scale.y = (float)transform["scale"][2];
			objectData1.scale.z = (float)transform["scale"][1];

			// コライダーのパラメータ読み込み
			nlohmann::json& collider = object["collider"];

			// コライダーの種類
			colliderData1.type = collider["type"];
			//コライダーの中心
			colliderData1.center.x = (float)collider["center"][0];
			colliderData1.center.y = (float)collider["center"][2];
			colliderData1.center.z = (float)collider["center"][1];
			//コライダーのサイズ
			colliderData1.scale.x = (float)collider["size"][0];
			colliderData1.scale.y = (float)collider["size"][2];
			colliderData1.scale.z = -(float)collider["size"][1];
			//コライダーの回転角
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
			//弧度法に変換
			colliderData1.rotation.x *= 1.0f / 360.0f * (2.0f * PI);
			colliderData1.rotation.y *= 1.0f / 360.0f * (2.0f * PI);
			colliderData1.rotation.z *= 1.0f / 360.0f * (2.0f * PI);

			//テクスチャデータ
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
			//シェーダ名
			if (object.contains("shaderName"))
			{
				textureData1.shaderName = object["shaderName"].get<std::string>();
			}
			else
			{
				textureData1.shaderName = "null";
			}

			// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
			/*if (object.contains("children")) {

			}*/

			//オブジェクトデータに代入
			objectData.push_back(objectData1);
			//コライダーデータに代入
			colliderData.push_back(colliderData1);
			//テクスチャデータに代入
			textureData.push_back(textureData1);

			//オブジェクトの数を増やす
			objectNum++;
		}
	}
}

std::vector<JSONLoader::EnemyPatern> JSONLoader::LoadEnemyPatern(const std::string fileName)
{
	//読み込んだ敵の行動パターンを入れておくコンテナ
	std::vector<EnemyPatern> enemeyPatern;

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
	assert(jsonFileList.contains("patern"));
	assert(jsonFileList["patern"].is_string());

	//"name"を文字列として取得
	std::string name = jsonFileList["patern"].get<std::string>();
	//"EnemyPatern"でなければ終了
	assert(name.compare("EnemyPatern") == 0);

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : jsonFileList["paterns"]) {

		//"status"がなければ終了
		assert(object.contains("status"));

		//状態を取得
		std::string status = object["status"].get<std::string>();
		std::string time = object["status"].get<std::string>();
		std::string nextStatus = object["nextStatus"].get<std::string>();

		//取得した状態を変換
		EnemyPatern patern;
		patern.status = status;
		patern.time = atoi(time.c_str());
		patern.nextStatus = nextStatus;

		//ステータスの番号を設定(状態が増える度変える必要あり)
		if (status == "Stand")patern.statusNum = 0;
		if (status == "Walk")patern.statusNum = 1;
		if (status == "Attack1")patern.statusNum = 2;
		//ステータスの番号を設定(状態が増える度変える必要あり)
		if (nextStatus == "Stand")patern.nextStatusNum = 0;
		if (nextStatus == "Walk")patern.nextStatusNum = 1;
		if (nextStatus == "Attack1")patern.nextStatusNum = 2;

		//コンテナに代入
		enemeyPatern.emplace_back(patern);
	}

	return enemeyPatern;
}
