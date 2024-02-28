/**
 * @file WinApp.h
 * @brief ウィンドウを生成、表示する
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once
#include "Windows.h"
#include "cstdint"

static const int window_width = 1280;	//横幅
static const int window_height = 720;	//立幅

namespace MyEngine
{
	class WinApp
	{
	public:
		//ウィンドウプロシージャ
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	public:

		/// <summary>
		///インスタンス取得
		/// </summary>
		static WinApp* GetInstance();

		/// <summary>
		///ウィンドウ生成
		/// </summary>
		void CreateWindow_(const wchar_t* title);

		/// <summary>
		///ウィンドウクラスを登録解除
		/// </summary>
		void deleteWindow();

		/// <summary>
		///hwnd取得
		/// </summary>
		HWND GetHwnd() { return hwnd; };
	public:
		//メンバ変数
		WNDCLASSEX w{};
		HWND hwnd;
	};
}