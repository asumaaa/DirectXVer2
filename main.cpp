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
	PostEffect* postEffect = nullptr;
	PostEffect::SetDevice(dxCommon->GetDevice());
	postEffect = new PostEffect;
	postEffect->Initialize();
	postEffect->LoadFile(0, L"Resources/white1x1.png");
	postEffect->CreateGraphicsPipeLine();

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
		postEffect->SetAlpha(1.0f);
		postEffect->SetScale({ 500.0f, 500.0 });
		postEffect->SetPosition({ 0.0f, 0.0 });
		postEffect->Update();

		dxCommon->PreDraw();

		// 4. 描画コマンド
		gameScene->Draw();

		//ポストエフェクト描画
		postEffect->Draw(dxCommon->GetCommandList());

		imGuiManager->End();
		imGuiManager->Draw();

		dxCommon->PostDraw();
	}

	fps->FpsControlEnd();

	dxCommon->EndImgui();

	imGuiManager->Finalize();

	//ウィンドウクラスを登録解除
	winApp->deleteWindow();

	return 0;
}
