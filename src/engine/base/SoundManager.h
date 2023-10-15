/**
 * @file SoundManager.h
 * @brief 音を管理
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <map>
#include <combaseapi.h>
#include <Windows.h>
#include <cassert>
#include <iterator>
#include <wrl.h>


//チャンクヘッダ
struct ChunkHeader {
	char id[4];		//チャンク毎のID
	int32_t size;	//チャンクサイズ
};

//RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk;	//"RIFF"
	char type[4];		//"WAVE"
};

//FMTチャンク
struct FormatChunk {
	ChunkHeader chunk;	//"fmt"
	WAVEFORMATEX fmt;	//波形フォーマット
};

class SoundManager final
{
public:
	SoundManager();
	~SoundManager();

public://静的メンバ変数

	static Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	static IXAudio2MasteringVoice* masterVoice_;

public://メンバ関数

	/// <summary>
	///コピーコンストラクタ無効
	/// </summary>
	SoundManager(const SoundManager& obj) = delete;

	/// <summary>
	///代入演算子を無効
	/// </summary>
	SoundManager& operator=(const SoundManager& obj) = delete;

	/// <summary>
	///インスタンス取得
	/// </summary>
	static SoundManager* GetInstance();

	/// <summary>
	///初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	///音声読み込み
	/// </summary>
	void SoundLoadWave(const char* filename);

	/// <summary>
	///音声再生
	/// </summary>
	void SoundPlayWave(bool loop = false, float volume = 1.0f);

	/// <summary>
	///音声停止
	/// </summary>
	void StopWave();

	/// <summary>
	///音声データ解放
	/// </summary>
	void SoundUnload();


private:
	//音声データ
	struct SoundData {
		//波形フォーマット
		WAVEFORMATEX wfex;
		//バッファの先頭アドレス
		BYTE* pBuffer;
		//バッファサイズ
		unsigned int bufferSize;
	};

	SoundData soundData_;

	IXAudio2SourceVoice* pSourceVoice = nullptr;

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};

	bool isPlay = false;


};