/**
 * @file ImGuiManager.h
 * @brief ImGuiManagerの管理
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"

class ImGuiManager
{
public:

	/// <summary>
	///初期化
	/// </summary>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	///終了時呼び出し処理
	/// </summary>
	void Finalize();

	/// <summary>
	///ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	///ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	///描画
	/// </summary>
	void Draw();


private:
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>srvHeap_;
};
