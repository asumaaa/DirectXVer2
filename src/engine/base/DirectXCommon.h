/**
 * @file DirectXCommon.h
 * @brief エンジンの基礎
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once
#include <Windows.h>
#include <cstdlib>
#include <d3d12.h>
//#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <algorithm>
#include <cassert>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include "WinApp.h"

using namespace Microsoft::WRL;

class DirectXCommon
{
private:

	//インストラクタ デストラクタ
	DirectXCommon() = default;
	~DirectXCommon();

public:

	/// <summary>
	///インスタンス取得
	/// </summary>
	static DirectXCommon* GetInstance();

	/// <summary>
	///初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	///初期化関連
	/// </summary>
	void InitializeDevice();			//デバイス関連
	void InitializeCommand();			//コマンド関連
	void InitializeSwapchain();			//スワップチェーン関連
	void InitializeRenderTargetView();	//レンダーターゲット関連
	void InitializeDepthBuffer();		//深度バッファ関連
	void InitializeFence();				//フェンス関連

	/// <summary>
	///描画前処理
	/// </summary>
	void PreDraw();	

	/// <summary>
	///描画前処理
	/// </summary>
	void PreDraw1();

	/// <summary>
	///描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	///描画後処理
	/// </summary>
	void PostDraw1();

	/// <summary>
	///imgui用のヒープ生成
	/// </summary>
	ComPtr<ID3D12DescriptorHeap>CreateDescriptorForImgui();

	/// <summary>
	///imgui初期化
	/// </summary>
	void InitializeImgui();

	/// <summary>
	///imgui終了
	/// </summary>
	void EndImgui();

	/// <summary>
	///デバイス取得
	/// </summary>
	ID3D12Device* GetDevice() { return device.Get(); }

	/// <summary>
	///スワップチェーン取得
	/// </summary>
	IDXGISwapChain4* GetSwapChain() { return swapChain.Get(); }

	/// <summary>
	///コマンドリスト取得
	/// </summary>
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }

	/// <summary>
	///コマンドアロケータ取得
	/// </summary>
	ID3D12CommandAllocator* GetCommandAllocator() { return commandAllocator.Get(); }

	/// <summary>
	///コマンドキュー取得
	/// </summary>
	ID3D12CommandQueue* GetCommandQueue() { return commandQueue.Get(); }

	/// <summary>
	///ヒープ取得
	/// </summary>
	ID3D12DescriptorHeap* GetRtvHeap() { return rtvHeap.Get(); }

	/// <summary>
	///フェンス取得
	/// </summary>
	ID3D12Fence* GetFence() { return fence.Get(); }

	/// <summary>
	///imgui用ヒープ取得
	/// </summary>
	ComPtr<ID3D12DescriptorHeap>GetHeapForImgui() { return _heapForImgui; };

	/// <summary>
	///バッファの数取得
	/// </summary>
	size_t GetBackBufferCount() const { return backBuffers.size(); }

public:
	//メンバ変数
	//ウィンドウ
	WinApp* winApp_;
private:
	//DirectX
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12Debug> debugController;
	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12Fence> fence;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Resource> depthBuff;
	D3D12_RESOURCE_DESC depthResorceDesc{};
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	D3D12_CLEAR_VALUE depthClearValue{};
	D3D12_RESOURCE_BARRIER barrierDesc{};
	//imgui用ヒープ
	ComPtr<ID3D12DescriptorHeap> _heapForImgui;
public:
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	std::vector<ComPtr<ID3D12Resource>>backBuffers;
	/*std::vector<ComPtr<IDXGIAdapter4>>adapters;
	ComPtr<IDXGIAdapter4> tmpAdapter;*/

	UINT64 fenceVal = 0;
};