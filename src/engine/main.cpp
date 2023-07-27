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

//#include "Effekseer.h"
//#include "Effekseer.Modules.h"
//#include "Effekseer.SIMD.h"
//#include "EffekseerRendererDX11.h"

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
