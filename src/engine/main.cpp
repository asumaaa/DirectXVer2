#include "WinApp.h"
#include "Message.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "DirectXTex.h"
#include "FPS.h"
#include "fbxsdk.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "imgui.h"
#include "MonochromeEffect.h"
#include "ReversalEffect.h"
#include "BlurEffect.h"
#include "MosaicEffect.h"
#include "ChromaticAberrationEffect.h"
#include "ShadowMap.h"
#include "DepthOfField.h"
#include "Fog.h"
#include "Vignette.h"
#include "Sprite.h"

#include "Effekseer.h"
#include "EffekseerRendererDX12.h"

#pragma comment(lib,"EffekseerRendererDX12.lib")
#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"LLGI.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ウィンドウ生成
	WinApp* winApp = nullptr;
	winApp = WinApp::GetInstance();
	winApp->CreateWindow_(L"あいうえお");

	//メッセージ
	Message* message;
	message = Message::GetInstance();

	//DirectX初期化処理
	DirectXCommon* dxCommon = nullptr;
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	//キーボード
	Input* input = nullptr;
	input = Input::GetInstance();
	input->Initialize(winApp);

	//ImGuiManager
	ImGuiManager* imGuiManager = nullptr;
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp,dxCommon);

	//ShadowMap
	ShadowMap* shadowMap = nullptr;
	ShadowMap::SetDevice(dxCommon->GetDevice());
	shadowMap = new ShadowMap;
	shadowMap->Initialize();
	shadowMap->CreateGraphicsPipeLine0();

	//ゲームシーン
	GameScene* gameScene = nullptr;
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon, input);

	//FPSを固定
	FPS* fps = nullptr;
	fps = new FPS;
	fps->SetFrameRate(60.0f);
	fps->FpsControlBegin();

	////エフェクトレンダラー
	//EffekseerRenderer::RendererRef efkRenderer;
	////エフェクトマネージャー
	//Effekseer::ManagerRef efkManager;
	////メモリプール
	//Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> efkMemoryPool;
	////コマンドリスト
	//Effekseer::RefPtr < EffekseerRenderer::CommandList> efkCmdList;
	////エフェクト本体
	//Effekseer::Effect* effect = nullptr;
	////エフェクトハンドル
	//Effekseer::Handle* efkHandle = nullptr;

	//DXGI_FORMAT bbFormats = DXGI_FORMAT_R8G8B8A8_UNORM;
	////エフェクトレンダラーの初期化
	//efkRenderer = EffekseerRendererDX12::Create(
	//	dxCommon->GetDevice(),
	//	dxCommon->GetCommandQueue(),
	//	2,
	//	&bbFormats,
	//	1,
	//	bbFormats,
	//	false,
	//	10000);
	//efkManager = Effekseer::Manager::Create(10000);
	////座標を左手系にする
	//efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	////描画用インスタンスから描画機能を設定
	//efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	//efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	//efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	//efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	//efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());
	////描画用インスタンスからテクスチャの読み込み昨日を設定
	////拡張機能も可能
	//efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
	//efkManager->SetModelLoader(efkRenderer->CreateModelLoader());
	////DirecrX12特有の処理
	//efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(efkRenderer);
	//efkCmdList = EffekseerRenderer::CreateCommandList(efkRenderer, efkMemoryPool);
	/*efkRenderer->SetCommandList(efkCmdList);*/

	//ゲームループ
	while (true)
	{
		//✖ボタンでゲームループ終了
		if (message->Update() == 1)break;

		//キーボード更新
		input->Update();

		imGuiManager->Begin();

		//ゲームシーン更新
		//shadowMap
		shadowMap->SetAlpha(1.0f);
		shadowMap->SetLightVP(gameScene->GetLightViewProjection());
		shadowMap->Update();

		//ゲームシーン
		gameScene->Update();

		//// 4. 描画コマンド
		//
		////レンダーテクスチャへの描画

		//shadowMap
		shadowMap->PreDrawScene0(dxCommon->GetCommandList());
		gameScene->DrawFBXLightView();
		shadowMap->PostDrawScene0(dxCommon->GetCommandList());
		//ゲームシーンにSRVを渡す
		gameScene->SetSRV(shadowMap->GetSRV());

		//描画前処理
		dxCommon->PreDraw();
	
		gameScene->Draw();

		//ImGui
		//ImGui::Begin("blur");
		//ImGui::SetWindowPos(ImVec2(0, 0));
		//ImGui::SetWindowSize(ImVec2(500, 150));
		///*ImGui::InputFloat3("lightDir", lightDir);*/
		//ImGui::InputFloat("blur Strength", blurStrength);
		//ImGui::InputFloat("blur Width Strength", blurWidthStrength);
		//ImGui::InputFloat("blur Height Strength", blurHeightStrength);
		//ImGui::End();

		imGuiManager->End();
		imGuiManager->Draw();

		//描画後処理
		dxCommon->PostDraw();
	}


	fps->FpsControlEnd();

	dxCommon->EndImgui();

	imGuiManager->Finalize();
	gameScene->Finalize();

	delete shadowMap;
	delete imGuiManager;
	delete fps;

	//ウィンドウクラスを登録解除
	winApp->deleteWindow();

	return 0;
}
