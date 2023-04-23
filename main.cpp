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
#include "MonochromeEffect.h"
#include "ReversalEffect.h"
#include "BlurEffect.h"
#include "MosaicEffect.h"
#include "ChromaticAberrationEffect.h"

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

	////単色エフェクト
	//MonochromeEffect* monochromeEffect = nullptr;
	//MonochromeEffect::SetDevice(dxCommon->GetDevice());
	//monochromeEffect = new MonochromeEffect;
	//monochromeEffect->Initialize();
	//monochromeEffect->CreateGraphicsPipeLine();

	////反転エフェクト
	//ReversalEffect* reversalEffect = nullptr;
	//ReversalEffect::SetDevice(dxCommon->GetDevice());
	//reversalEffect = new ReversalEffect;
	//reversalEffect->Initialize();
	//reversalEffect->CreateGraphicsPipeLine();

	////ぼかしエフェクト
	//BlurEffect* blurEffect = nullptr;
	//BlurEffect::SetDevice(dxCommon->GetDevice());
	//blurEffect = new BlurEffect;
	//blurEffect->Initialize();
	//blurEffect->CreateGraphicsPipeLine();

	////モザイクエフェクト
	//MosaicEffect* mosaicEffect = nullptr;
	//MosaicEffect::SetDevice(dxCommon->GetDevice());
	//mosaicEffect = new MosaicEffect;
	//mosaicEffect->Initialize();
	//mosaicEffect->CreateGraphicsPipeLine();

	//RGBずらし
	ChromaticAberration* chromaticAberration = nullptr;
	ChromaticAberration::SetDevice(dxCommon->GetDevice());
	chromaticAberration = new ChromaticAberration;
	chromaticAberration->Initialize();
	chromaticAberration->CreateGraphicsPipeLine();

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

		////単色エフェクト
		//monochromeEffect->SetAlpha(1.0f);
		//monochromeEffect->SetColor({ 1.0f,1.0f,0.0 });
		//monochromeEffect->Update();

		////反転エフェクト
		//reversalEffect->SetAlpha(1.0f);
		//reversalEffect->Update();

		////ぼかしエフェクト
		//blurEffect->SetAlpha(1.0f);
		//blurEffect->SetResolution(20.0f);
		//blurEffect->Update();

		////モザイクエフェクト
		//mosaicEffect->SetAlpha(1.0f);
		//mosaicEffect->SetResolution(20.0f);
		//mosaicEffect->Update();

		//RGBずらし
		chromaticAberration->SetAlpha(1.0f);
		chromaticAberration->SetStrength(0.05f);
		chromaticAberration->Update();

		// 4. 描画コマンド
		
		//レンダーテクスチャへの描画

		////単色エフェクト
		//monochromeEffect->PreDrawScene(dxCommon->GetCommandList());
		//gameScene->Draw();
		//monochromeEffect->PostDrawScene(dxCommon->GetCommandList());
		////反転エフェクト
		//reversalEffect->PreDrawScene(dxCommon->GetCommandList());
		//gameScene->Draw();
		//reversalEffect->PostDrawScene(dxCommon->GetCommandList());
		////ぼかしエフェクト
		//blurEffect->PreDrawScene(dxCommon->GetCommandList());
		//gameScene->Draw();
		//blurEffect->PostDrawScene(dxCommon->GetCommandList());
		////モザイクエフェクト
		//mosaicEffect->PreDrawScene(dxCommon->GetCommandList());
		//gameScene->Draw();
		//mosaicEffect->PostDrawScene(dxCommon->GetCommandList());
		//RGBずらし
		chromaticAberration->PreDrawScene(dxCommon->GetCommandList());
		gameScene->Draw();
		chromaticAberration->PostDrawScene(dxCommon->GetCommandList());

		//描画前処理
		dxCommon->PreDraw();

		//描画開始
		//単色エフェクト
		//monochromeEffect->Draw(dxCommon->GetCommandList());
		//反転エフェクト
		//reversalEffect->Draw(dxCommon->GetCommandList());
		////ぼかしエフェクト 
		//blurEffect->Draw(dxCommon->GetCommandList());
		//モザイクエフェクト 
		//mosaicEffect->Draw(dxCommon->GetCommandList());
		//RGBずらし 
		chromaticAberration->Draw(dxCommon->GetCommandList());

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
