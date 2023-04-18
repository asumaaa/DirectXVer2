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
#include "PostEffect.h"

#include "Sprite.h"

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

	//ライト静的初期化
	LightGroup::StaticInitialize(dxCommon->GetDevice());

	//ポストエフェクト
	PostEffect* postEffect0 = nullptr;
	PostEffect* postEffect1 = nullptr;
	PostEffect::SetDevice(dxCommon->GetDevice());
	postEffect0 = new PostEffect;
	postEffect0->Initialize();
	postEffect0->CreateGraphicsPipeLine();

	postEffect1 = new PostEffect;
	postEffect1->Initialize();
	postEffect1->CreateGraphicsPipeLine();

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
		gameScene->Update();

		//ポストエフェクト
		postEffect0->SetAlpha(1.0f);
		postEffect0->SetScale({ window_width * 0.5f, window_height * 0.5f });
		postEffect0->SetPosition({ 0.0f, 0.0 });
		postEffect0->Update();

		postEffect1->SetAlpha(1.0f);
		postEffect1->SetScale({ window_width * 0.5f, window_height * 0.5f });
		postEffect1->SetPosition({ window_width * 0.5f, window_height * 0.5f });
		postEffect1->Update();

		// 4. 描画コマンド
		
		//レンダーテクスチャへの描画
		postEffect0->PreDrawScene(dxCommon->GetCommandList());
		gameScene->Draw();
		postEffect0->PostDrawScene(dxCommon->GetCommandList());

		postEffect1->PreDrawScene(dxCommon->GetCommandList());
		gameScene->Draw();
		postEffect1->PostDrawScene(dxCommon->GetCommandList());

		//描画前処理
		dxCommon->PreDraw();
		//描画開始
		postEffect0->Draw(dxCommon->GetCommandList());
		postEffect1->Draw(dxCommon->GetCommandList());

		imGuiManager->End();
		imGuiManager->Draw();

		//描画後処理
		dxCommon->PostDraw();
	}

	fps->FpsControlEnd();

	dxCommon->EndImgui();

	imGuiManager->Finalize();

	//ウィンドウクラスを登録解除
	winApp->deleteWindow();

	return 0;
}
