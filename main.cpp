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

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ウィンドウ生成
	WinApp* winApp = nullptr;
	winApp = WinApp::GetInstance();
	winApp->CreateWindow_(L"CG5_Test02");

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

	//ぼかしエフェクト
	BlurEffect* blurEffect = nullptr;
	BlurEffect::SetDevice(dxCommon->GetDevice());
	blurEffect = new BlurEffect;
	blurEffect->Initialize();
	blurEffect->CreateGraphicsPipeLine();
	float blurStrength[1] = { 5.0f };
	float blurWidthStrength[1] = { 10.0f };
	float blurHeightStrength[1] = { 10.0f };

	//被写界深度
	DepthOfField* depthOfField = nullptr;
	DepthOfField::SetDevice(dxCommon->GetDevice());
	depthOfField = new DepthOfField;
	depthOfField->Initialize();
	depthOfField->CreateGraphicsPipeLine();
	float depthOfFieldFocus[1] = { 0.1 };
	float depthOfFieldFNumber[1] = { 0.3 };
	float depthOfFieldStrength[1] = { 50 };


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

	//ポストエフェクト切り替えようフラグ
	int blurFlag[1] = { 1 };
	int depthOfFieldFlag[1] = { 0 };

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
		
		//ぼかしエフェクト
		blurEffect->SetAlpha(1.0f);
		blurEffect->SetWidthStrength(*blurWidthStrength);
		blurEffect->SetHeightStrength(*blurHeightStrength);
		blurEffect->Update();

		//被写界深度
		depthOfField->SetAlpha(1.0f);
		depthOfField->SetFocus(*depthOfFieldFocus);
		depthOfField->SetFNumber(*depthOfFieldFNumber);
		depthOfField->SetStrength(*depthOfFieldStrength);
		depthOfField->Update();

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

		//ぼかし
		blurEffect->PreDrawScene(dxCommon->GetCommandList());
		gameScene->Draw();
		blurEffect->PostDrawScene(dxCommon->GetCommandList());

		//被写界深度
		depthOfField->PreDrawScene(dxCommon->GetCommandList());
		gameScene->Draw();
		depthOfField->PostDrawScene(dxCommon->GetCommandList());

		//描画前処理
		dxCommon->PreDraw();


		if(*blurFlag == 1)blurEffect->Draw(dxCommon->GetCommandList());
		else if (*depthOfFieldFlag == 1)depthOfField->Draw(dxCommon->GetCommandList());
		else gameScene->Draw();

		//ImGui
		ImGui::Begin("blur");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(500, 250));
		/*ImGui::InputFloat3("lightDir", lightDir);*/
		ImGui::InputInt("blurFlag", blurFlag);
		ImGui::InputFloat("blur Width Strength", blurWidthStrength);
		ImGui::InputFloat("blur Height Strength", blurHeightStrength);
		ImGui::InputInt("depthOfFieldFlag", depthOfFieldFlag);
		ImGui::InputFloat("depthOfFieldFocus", depthOfFieldFocus);
		ImGui::InputFloat("depthOfFieldFNumber", depthOfFieldFNumber);
		ImGui::InputFloat("depthOfFieldStrength", depthOfFieldStrength);
		ImGui::End();

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
