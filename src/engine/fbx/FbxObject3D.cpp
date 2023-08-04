#include "FbxObject3D.h"
#include "FbxLoader.h"
#include <iostream>
#include <locale> 
#include <codecvt> 
#include <cstdio>
#include "mathOriginal.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

//ライト視点のルートシグネチャとパイプライン(shadowMap用)
ComPtr<ID3D12RootSignature>FbxObject3D::rootsignature0;
ComPtr<ID3D12PipelineState>FbxObject3D::pipelinestate0;
//影付きカメラ視点のルートシグネチャとパイプライン
ComPtr<ID3D12RootSignature>FbxObject3D::rootsignature2;
ComPtr<ID3D12PipelineState>FbxObject3D::pipelinestate2;

ID3D12Device* FbxObject3D::device = nullptr;
Camera* FbxObject3D::camera = nullptr;
Light* FbxObject3D::light = nullptr;
LightGroup* FbxObject3D::lightGroup = nullptr;

void FbxObject3D::Initialize()
{
	//1フレーム分の時間を60FPSで設定
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	HRESULT result;
	//定数バッファの生成
	CD3DX12_HEAP_PROPERTIES v1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC v2 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);
	CD3DX12_RESOURCE_DESC v3 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);
	result = device->CreateCommittedResource(
		&v1,
		D3D12_HEAP_FLAG_NONE,
		&v2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	result = device->CreateCommittedResource(
		&v1,	//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&v3,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin)
	);

	//定数バッファへデータ転送
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin->Unmap(0, nullptr);

}

void FbxObject3D::Update()
{
	HRESULT result;
	//アニメーション
	if (isPlay)
	{
		//1フレーム進める
		currentTime += frameTime;
		//最後まで再生したら先頭に戻す
		if (currentTime > endTime)
		{
			currentTime = startTime;
		}
	}

	//ボーン配列
	std::vector<FbxModel::Bone>& bones = model->GetBones();
	//定数バッファへデータ転送
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//今の姿勢行列
		XMMATRIX matCurrentPose/* = XMMatrixIdentity()*/;
		//今の姿勢行列を取得
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX1に変換
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//合成してスキニング行列に
		constMapSkin->bones[i] = model->GetModelTransform() * bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

	XMMATRIX matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//ワールド行列の生成
	matWorld = XMMatrixIdentity();

	//ビルボードの行列更新
	UpdateBillboard();
	//ビルボードフラグが立っていれば
	if (billboardFlag == true)
	{
		matWorld *= matBillboard;
	}

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;
	
	//射影返還
	const XMMATRIX& matView = camera->GetMatView();
	//ビュープロジェクション行列
	const XMMATRIX& matViewProjection = camera->GetMatViewProjection();
	//モデルのメッシュトランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = camera->GetEye();

	//定数バッファへデータ転送
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = matWorld;
		constMap->cameraPos = cameraPos;
		constMap->lightviewproj = light->GetMatViewProjection();
		constBuffTransform->Unmap(0, nullptr);
	}

	//コライダー更新
	UpdateCollider();
}

void FbxObject3D::UpdateBillboard()
{
	XMFLOAT3 eye = camera->GetEye();
	XMFLOAT3 target = camera->GetTarget();
	XMFLOAT3 up = camera->GetUp();
	//視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	//注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	//(仮の)上方向
	XMVECTOR upVector = XMLoadFloat3(&up);

	//カメラZ軸
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	//0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	//ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//カメラのX軸(右方向)
	XMVECTOR cameraAxisX;
	//X軸は上方向→Z軸の外積で決まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//カメラのY軸
	XMVECTOR cameraAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	////カメラ回転行列
	//XMMATRIX matCameraRot;
	////カメラ座標系→ワールド座標系の変換行列
	//matCameraRot.r[0] = cameraAxisX;
	//matCameraRot.r[1] = cameraAxisY;
	//matCameraRot.r[2] = cameraAxisZ;
	//matCameraRot.r[3] = XMVectorSet(0,0,0,1);

	////転置により逆行列(逆回転)を計算
	//matView = XMMatrixTranspose(matCameraRot);

	////視点座標に-1をかけた値
	//XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	////カメラの位置からワールド原点へのベクトル
	//XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	//XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	//XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	////一つのベクトルにまとめる
	//XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//matView.r[3] = translation;


	//全方向ビルボード行列の計算
	//ビルボード行列
	matBillboard = XMMatrixIdentity();
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
}

void FbxObject3D::UpdateCollider()
{
	colliderPos1 = position - colliderPos0;
	colliderData.center = colliderCenter + colliderPos1;
	colliderData.rotation = colliderRotation;
	colliderData.scale = colliderScale;
}

void FbxObject3D::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	//モデルの割り当てがなければ描画市内
	if (model == nullptr)
	{
		return;
	}

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate0.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature0.Get());
	//プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	//モデル描画
	model->Draw(cmdList,textureNum1);
}

void FbxObject3D::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//モデルの割り当てがなければ描画市内
	if (model == nullptr)
	{
		return;
	}

	//シェーダを設定していない場合
	if (shaderFlag == false)
	{
		//パイプラインステートの設定
		cmdList->SetPipelineState(pipelinestate2.Get());
		//ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(rootsignature2.Get());
	}
	//シェーダを指定している場合
	else
	{
		//パイプラインステートの設定
		cmdList->SetPipelineState(pipelinestate1.Get());
		//ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(rootsignature1.Get());
	}
	//プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());
	//ライト描画
	lightGroup->Draw(cmdList, 5);

	//深度値をセット
	cmdList->SetDescriptorHeaps(1, &depthSRV);
	D3D12_GPU_DESCRIPTOR_HANDLE handle = depthSRV->GetGPUDescriptorHandleForHeapStart();
	cmdList->SetGraphicsRootDescriptorTable(3, handle);
	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cmdList->SetGraphicsRootDescriptorTable(4, handle);

	//モデル描画
	if (textureVol == 1)model->DrawTexture1(cmdList, textureNum1);
	else if (textureVol == 2)model->DrawTexture2(cmdList, textureNum1, textureNum2);
	else if (textureVol == 3)model->DrawTexture3(cmdList, textureNum1, textureNum2, textureNum3);
	else if (textureVol == 4)model->DrawTexture4(cmdList, textureNum1, textureNum2, textureNum3, textureNum4);

	model->PreDraw(cmdList);
}

void FbxObject3D::CreateGraphicsPipelineLightView()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device);

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/Fbx/FbxVertexShader0.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/Fbx/FbxPixelShader0.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//影響を受けるボーン番号(4つ)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//ボーンのスキンウェイト(4つ)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(スキニング)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), 
		IID_PPV_ARGS(rootsignature0.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature0.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, 
		IID_PPV_ARGS(pipelinestate0.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device);

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/Fbx/FbxVertexShader2.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/Fbx/FbxPixelShader2.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//影響を受けるボーン番号(4つ)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//ボーンのスキンウェイト(4つ)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = false;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[6];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(スキニング)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//テクスチャレジスタ1番
	rootparams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootparams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//テクスチャレジスタ1番
	rootparams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[4].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootparams[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//定数バッファ(ライト)
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootsignature2.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature2.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(pipelinestate2.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::CreateGraphicsPipelineTexture1()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device);

	//ファイル名設定
	std::string baseDirectory = "Resources/Shaders/";
	std::string vFileName = "VertexShader.hlsl";
	std::string pFileName = "PixelShader.hlsl";
	std::string fileName = shaderName + "/" + shaderName;
	std::string vertexFileName0 = baseDirectory + fileName + vFileName;
	std::string pixelFileName0 = baseDirectory + fileName + pFileName;

	//string→wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> v;
	std::wstring vertexFileName1 = v.from_bytes(vertexFileName0);
	std::wcout << vertexFileName1 << std::endl;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> p;
	std::wstring pixelFileName1 = p.from_bytes(pixelFileName0);
	std::wcout << pixelFileName1 << std::endl;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		vertexFileName1.c_str(),    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		pixelFileName1.c_str(),    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//影響を受けるボーン番号(4つ)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//ボーンのスキンウェイト(4つ)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = false;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[6];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(スキニング)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//テクスチャレジスタ1番
	rootparams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootparams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//テクスチャレジスタ1番
	rootparams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[4].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootparams[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//定数バッファ(ライト)
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootsignature1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature1.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(pipelinestate1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::CreateGraphicsPipelineTexture2()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device);

	//ファイル名設定
	std::string baseDirectory = "Resources/Shaders/";
	std::string vFileName = "VertexShader.hlsl";
	std::string pFileName = "PixelShader.hlsl";
	std::string fileName = shaderName + "/" + shaderName;
	std::string vertexFileName0 = baseDirectory + fileName + vFileName;
	std::string pixelFileName0 = baseDirectory + fileName + pFileName;

	//string→wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> v;
	std::wstring vertexFileName1 = v.from_bytes(vertexFileName0);
	std::wcout << vertexFileName1 << std::endl;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> p;
	std::wstring pixelFileName1 = p.from_bytes(pixelFileName0);
	std::wcout << pixelFileName1 << std::endl;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		vertexFileName1.c_str(),    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		pixelFileName1.c_str(),    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//影響を受けるボーン番号(4つ)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//ボーンのスキンウェイト(4つ)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = false;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // t3 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[7];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(スキニング)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//テクスチャレジスタ1番
	rootparams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootparams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//テクスチャレジスタ1番
	rootparams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[4].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootparams[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//定数バッファ(ライト)
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ2枚目）
	rootparams[6].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootsignature1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature1.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(pipelinestate1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::CreateGraphicsPipelineTexture3()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device);

	//ファイル名設定
	std::string baseDirectory = "Resources/Shaders/";
	std::string vFileName = "VertexShader.hlsl";
	std::string pFileName = "PixelShader.hlsl";
	std::string fileName = shaderName + "/" + shaderName;
	std::string vertexFileName0 = baseDirectory + fileName + vFileName;
	std::string pixelFileName0 = baseDirectory + fileName + pFileName;

	//string→wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> v;
	std::wstring vertexFileName1 = v.from_bytes(vertexFileName0);
	std::wcout << vertexFileName1 << std::endl;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> p;
	std::wstring pixelFileName1 = p.from_bytes(pixelFileName0);
	std::wcout << pixelFileName1 << std::endl;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		vertexFileName1.c_str(),    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		pixelFileName1.c_str(),    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//影響を受けるボーン番号(4つ)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//ボーンのスキンウェイト(4つ)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = false;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // t3 レジスタ

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV2;
	descRangeSRV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4); // t4 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[8];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(スキニング)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//テクスチャレジスタ1番
	rootparams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootparams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//テクスチャレジスタ1番
	rootparams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[4].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootparams[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//定数バッファ(ライト)
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ2枚目）
	rootparams[6].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ3枚目）
	rootparams[7].InitAsDescriptorTable(1, &descRangeSRV2, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootsignature1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature1.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(pipelinestate1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::CreateGraphicsPipelineTexture4()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device);

	//ファイル名設定
	std::string baseDirectory = "Resources/Shaders/";
	std::string vFileName = "VertexShader.hlsl";
	std::string pFileName = "PixelShader.hlsl";
	std::string fileName = shaderName + "/" + shaderName;
	std::string vertexFileName0 = baseDirectory + fileName + vFileName;
	std::string pixelFileName0 = baseDirectory + fileName + pFileName;

	//string→wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> v;
	std::wstring vertexFileName1 = v.from_bytes(vertexFileName0);
	std::wcout << vertexFileName1 << std::endl;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> p;
	std::wstring pixelFileName1 = p.from_bytes(pixelFileName0);
	std::wcout << pixelFileName1 << std::endl;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		vertexFileName1.c_str(),    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		pixelFileName1.c_str(),    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//影響を受けるボーン番号(4つ)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//ボーンのスキンウェイト(4つ)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = false;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 5;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // t3 レジスタ

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV2;
	descRangeSRV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4); // t4 レジスタ

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV3;
	descRangeSRV3.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5); // t5 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[9];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(スキニング)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//テクスチャレジスタ1番
	rootparams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootparams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//テクスチャレジスタ1番
	rootparams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[4].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootparams[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//定数バッファ(ライト)
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ2枚目）
	rootparams[6].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ3枚目）
	rootparams[7].InitAsDescriptorTable(1, &descRangeSRV2, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ3枚目）
	rootparams[8].InitAsDescriptorTable(1, &descRangeSRV3, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootsignature1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature1.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(pipelinestate1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::PlayAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//0番アニメーション取得
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	//アニメーションの名前取得
	const char* animstackname = animstack->GetName();
	//アニメーションの時間情報
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	//開始時間取得
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//終了時間取得
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//開始時間に合わせる
	currentTime = startTime;
	//再生中状態にする
	isPlay = true;
}

void FbxObject3D::SetObjectData(JSONLoader::ObjectData objectData)
{
	position = objectData.position;
	colliderPos0 = objectData.position;
	scale = objectData.scale;
	rotation = objectData.rotation;
	fileName = objectData.fileName;
	objectName = objectData.objectName;
}

void FbxObject3D::SetColliderData(JSONLoader::ColliderData colliderData)
{
	FbxObject3D::colliderData.type = colliderData.type;
	FbxObject3D::colliderData.objectName = colliderData.objectName;
	FbxObject3D::colliderData.scale = colliderData.scale;
	FbxObject3D::colliderData.center = colliderData.center;
	FbxObject3D::colliderData.rotation = colliderData.rotation;
	colliderRotation = colliderData.rotation;
	colliderCenter = colliderData.center;
	colliderScale = colliderData.scale;
}

void FbxObject3D::SetTextureData(JSONLoader::TextureData textureData)
{
	textureNum1 = textureData.textureNum1;
	if (textureData.textureVol == 1)
	{
		//テクスチャ1枚でテクスチャを設定していたら
		if (textureData.shaderName != "null")
		{
			shaderName = textureData.shaderName;
			//パイプライン生成
			CreateGraphicsPipelineTexture1();
			shaderFlag = true;
		}
	}
	if (textureData.textureVol == 2)
	{
		textureNum1 = textureData.textureNum1;
		textureNum2 = textureData.textureNum2;
		textureVol = 2;
		shaderName = textureData.shaderName;
		CreateGraphicsPipelineTexture2();
		shaderFlag = true;
	}
	if (textureData.textureVol == 3)
	{
		textureNum1 = textureData.textureNum1;
		textureNum2 = textureData.textureNum2;
		textureNum3 = textureData.textureNum3;
		textureVol = 3;
		shaderName = textureData.shaderName;
		CreateGraphicsPipelineTexture3();
		shaderFlag = true;
	}
	if (textureData.textureVol == 4)
	{
		textureNum1 = textureData.textureNum1;
		textureNum2 = textureData.textureNum2;
		textureNum3 = textureData.textureNum3;
		textureNum4 = textureData.textureNum4;
		textureVol = 4;
		shaderName = textureData.shaderName;
		CreateGraphicsPipelineTexture4();
		shaderFlag = true;
	}
}

