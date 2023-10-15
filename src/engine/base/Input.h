/**
 * @file Input.h
 * @brief キーボードによる入力を管理
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once

#include <array>
#include <vector>
#include <wrl.h>

#include <XInput.h>
#include "WinApp.h"

#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
using namespace Microsoft::WRL;
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Input
{
public:

	/// <summary>
	///インスタンス取得
	/// </summary>
	static Input* GetInstance();

	/// <summary>
	///初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	///更新
	/// </summary>
	void Update();

	/// <summary>
	///キー取得
	/// </summary>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	///トリガーキー取得
	/// </summary>
	bool TriggerKey(BYTE keyNumber);

public:
	//ウィンドウ
	WinApp* winApp_;
	ComPtr<IDirectInput8> directInput;
	//キーボード
	ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
};