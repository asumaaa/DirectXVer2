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

//���C�g���_�̃��[�g�V�O�l�`���ƃp�C�v���C��(shadowMap�p)
ComPtr<ID3D12RootSignature>FbxObject3D::rootsignature0;
ComPtr<ID3D12PipelineState>FbxObject3D::pipelinestate0;
//�e�t���J�������_�̃��[�g�V�O�l�`���ƃp�C�v���C��
ComPtr<ID3D12RootSignature>FbxObject3D::rootsignature2;
ComPtr<ID3D12PipelineState>FbxObject3D::pipelinestate2;

ID3D12Device* FbxObject3D::device = nullptr;
Camera* FbxObject3D::camera = nullptr;
Light* FbxObject3D::light = nullptr;
LightGroup* FbxObject3D::lightGroup = nullptr;

void FbxObject3D::Initialize()
{
	//1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	HRESULT result;
	//�萔�o�b�t�@�̐���
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
		&v1,	//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&v3,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin)
	);

	//�萔�o�b�t�@�փf�[�^�]��
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
	//�A�j���[�V����
	if (isPlay)
	{
		//1�t���[���i�߂�
		currentTime += frameTime;
		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (currentTime > endTime)
		{
			currentTime = startTime;
		}
	}

	//�{�[���z��
	std::vector<FbxModel::Bone>& bones = model->GetBones();
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//���̎p���s��
		XMMATRIX matCurrentPose/* = XMMatrixIdentity()*/;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX1�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = model->GetModelTransform() * bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̐���
	matWorld = XMMatrixIdentity();

	//�r���{�[�h�̍s��X�V
	UpdateBillboard();
	//�r���{�[�h�t���O�������Ă����
	if (billboardFlag == true)
	{
		matWorld *= matBillboard;
	}

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;
	
	//�ˉe�Ԋ�
	const XMMATRIX& matView = camera->GetMatView();
	//�r���[�v���W�F�N�V�����s��
	const XMMATRIX& matViewProjection = camera->GetMatViewProjection();
	//���f���̃��b�V���g�����X�t�H�[��
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//�J�������W
	const XMFLOAT3& cameraPos = camera->GetEye();

	//�萔�o�b�t�@�փf�[�^�]��
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

	//�R���C�_�[�X�V
	UpdateCollider();
}

void FbxObject3D::UpdateBillboard()
{
	XMFLOAT3 eye = camera->GetEye();
	XMFLOAT3 target = camera->GetTarget();
	XMFLOAT3 up = camera->GetUp();
	//���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	//�����_���W
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	//(����)�����
	XMVECTOR upVector = XMLoadFloat3(&up);

	//�J����Z��
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	//0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	//�x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//�J������X��(�E����)
	XMVECTOR cameraAxisX;
	//X���͏������Z���̊O�ςŌ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//�x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//�J������Y��
	XMVECTOR cameraAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	////�J������]�s��
	//XMMATRIX matCameraRot;
	////�J�������W�n�����[���h���W�n�̕ϊ��s��
	//matCameraRot.r[0] = cameraAxisX;
	//matCameraRot.r[1] = cameraAxisY;
	//matCameraRot.r[2] = cameraAxisZ;
	//matCameraRot.r[3] = XMVectorSet(0,0,0,1);

	////�]�u�ɂ��t�s��(�t��])���v�Z
	//matView = XMMatrixTranspose(matCameraRot);

	////���_���W��-1���������l
	//XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	////�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��
	//XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	//XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	//XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	////��̃x�N�g���ɂ܂Ƃ߂�
	//XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//matView.r[3] = translation;


	//�S�����r���{�[�h�s��̌v�Z
	//�r���{�[�h�s��
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
	//���f���̊��蓖�Ă��Ȃ���Ε`��s��
	if (model == nullptr)
	{
		return;
	}

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate0.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature0.Get());
	//�v���~�e�B�u�`��̐ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	//���f���`��
	model->Draw(cmdList,textureNum1);
}

void FbxObject3D::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//���f���̊��蓖�Ă��Ȃ���Ε`��s��
	if (model == nullptr)
	{
		return;
	}

	//�V�F�[�_��ݒ肵�Ă��Ȃ��ꍇ
	if (shaderFlag == false)
	{
		//�p�C�v���C���X�e�[�g�̐ݒ�
		cmdList->SetPipelineState(pipelinestate2.Get());
		//���[�g�V�O�l�`���̐ݒ�
		cmdList->SetGraphicsRootSignature(rootsignature2.Get());
	}
	//�V�F�[�_���w�肵�Ă���ꍇ
	else
	{
		//�p�C�v���C���X�e�[�g�̐ݒ�
		cmdList->SetPipelineState(pipelinestate1.Get());
		//���[�g�V�O�l�`���̐ݒ�
		cmdList->SetGraphicsRootSignature(rootsignature1.Get());
	}
	//�v���~�e�B�u�`��̐ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());
	//���C�g�`��
	lightGroup->Draw(cmdList, 5);

	//�[�x�l���Z�b�g
	cmdList->SetDescriptorHeaps(1, &depthSRV);
	D3D12_GPU_DESCRIPTOR_HANDLE handle = depthSRV->GetGPUDescriptorHandleForHeapStart();
	cmdList->SetGraphicsRootDescriptorTable(3, handle);
	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cmdList->SetGraphicsRootDescriptorTable(4, handle);

	//���f���`��
	if (textureVol == 1)model->DrawTexture1(cmdList, textureNum1);
	else if (textureVol == 2)model->DrawTexture2(cmdList, textureNum1, textureNum2);
	else if (textureVol == 3)model->DrawTexture3(cmdList, textureNum1, textureNum2, textureNum3);
	else if (textureVol == 4)model->DrawTexture4(cmdList, textureNum1, textureNum2, textureNum3, textureNum4);

	model->PreDraw(cmdList);
}

void FbxObject3D::CreateGraphicsPipelineLightView()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	assert(device);

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/Fbx/FbxVertexShader0.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/Fbx/FbxPixelShader0.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//�e�����󂯂�{�[���ԍ�(4��)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//�{�[���̃X�L���E�F�C�g(4��)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(�X�L�j���O)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), 
		IID_PPV_ARGS(rootsignature0.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature0.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, 
		IID_PPV_ARGS(pipelinestate0.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	assert(device);

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/Fbx/FbxVertexShader2.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/Fbx/FbxPixelShader2.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//�e�����󂯂�{�[���ԍ�(4��)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//�{�[���̃X�L���E�F�C�g(4��)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = false;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[6];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(�X�L�j���O)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//�e�N�X�`�����W�X�^1��
	rootparams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootparams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�e�N�X�`�����W�X�^1��
	rootparams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[4].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootparams[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�萔�o�b�t�@(���C�g)
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootsignature2.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature2.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(pipelinestate2.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::CreateGraphicsPipelineTexture1()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	assert(device);

	//�t�@�C�����ݒ�
	std::string baseDirectory = "Resources/Shaders/";
	std::string vFileName = "VertexShader.hlsl";
	std::string pFileName = "PixelShader.hlsl";
	std::string fileName = shaderName + "/" + shaderName;
	std::string vertexFileName0 = baseDirectory + fileName + vFileName;
	std::string pixelFileName0 = baseDirectory + fileName + pFileName;

	//string��wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> v;
	std::wstring vertexFileName1 = v.from_bytes(vertexFileName0);
	std::wcout << vertexFileName1 << std::endl;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> p;
	std::wstring pixelFileName1 = p.from_bytes(pixelFileName0);
	std::wcout << pixelFileName1 << std::endl;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		vertexFileName1.c_str(),    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		pixelFileName1.c_str(),    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//�e�����󂯂�{�[���ԍ�(4��)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//�{�[���̃X�L���E�F�C�g(4��)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = false;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[6];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(�X�L�j���O)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//�e�N�X�`�����W�X�^1��
	rootparams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootparams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�e�N�X�`�����W�X�^1��
	rootparams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[4].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootparams[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�萔�o�b�t�@(���C�g)
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootsignature1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature1.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(pipelinestate1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::CreateGraphicsPipelineTexture2()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	assert(device);

	//�t�@�C�����ݒ�
	std::string baseDirectory = "Resources/Shaders/";
	std::string vFileName = "VertexShader.hlsl";
	std::string pFileName = "PixelShader.hlsl";
	std::string fileName = shaderName + "/" + shaderName;
	std::string vertexFileName0 = baseDirectory + fileName + vFileName;
	std::string pixelFileName0 = baseDirectory + fileName + pFileName;

	//string��wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> v;
	std::wstring vertexFileName1 = v.from_bytes(vertexFileName0);
	std::wcout << vertexFileName1 << std::endl;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> p;
	std::wstring pixelFileName1 = p.from_bytes(pixelFileName0);
	std::wcout << pixelFileName1 << std::endl;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		vertexFileName1.c_str(),    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		pixelFileName1.c_str(),    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//�e�����󂯂�{�[���ԍ�(4��)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//�{�[���̃X�L���E�F�C�g(4��)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = false;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // t3 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[7];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(�X�L�j���O)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//�e�N�X�`�����W�X�^1��
	rootparams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootparams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�e�N�X�`�����W�X�^1��
	rootparams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[4].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootparams[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�萔�o�b�t�@(���C�g)
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`��2���ځj
	rootparams[6].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootsignature1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature1.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(pipelinestate1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::CreateGraphicsPipelineTexture3()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	assert(device);

	//�t�@�C�����ݒ�
	std::string baseDirectory = "Resources/Shaders/";
	std::string vFileName = "VertexShader.hlsl";
	std::string pFileName = "PixelShader.hlsl";
	std::string fileName = shaderName + "/" + shaderName;
	std::string vertexFileName0 = baseDirectory + fileName + vFileName;
	std::string pixelFileName0 = baseDirectory + fileName + pFileName;

	//string��wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> v;
	std::wstring vertexFileName1 = v.from_bytes(vertexFileName0);
	std::wcout << vertexFileName1 << std::endl;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> p;
	std::wstring pixelFileName1 = p.from_bytes(pixelFileName0);
	std::wcout << pixelFileName1 << std::endl;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		vertexFileName1.c_str(),    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		pixelFileName1.c_str(),    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//�e�����󂯂�{�[���ԍ�(4��)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//�{�[���̃X�L���E�F�C�g(4��)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = false;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // t3 ���W�X�^

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV2;
	descRangeSRV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4); // t4 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[8];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(�X�L�j���O)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//�e�N�X�`�����W�X�^1��
	rootparams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootparams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�e�N�X�`�����W�X�^1��
	rootparams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[4].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootparams[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�萔�o�b�t�@(���C�g)
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`��2���ځj
	rootparams[6].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`��3���ځj
	rootparams[7].InitAsDescriptorTable(1, &descRangeSRV2, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootsignature1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature1.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(pipelinestate1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::CreateGraphicsPipelineTexture4()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	assert(device);

	//�t�@�C�����ݒ�
	std::string baseDirectory = "Resources/Shaders/";
	std::string vFileName = "VertexShader.hlsl";
	std::string pFileName = "PixelShader.hlsl";
	std::string fileName = shaderName + "/" + shaderName;
	std::string vertexFileName0 = baseDirectory + fileName + vFileName;
	std::string pixelFileName0 = baseDirectory + fileName + pFileName;

	//string��wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> v;
	std::wstring vertexFileName1 = v.from_bytes(vertexFileName0);
	std::wcout << vertexFileName1 << std::endl;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> p;
	std::wstring pixelFileName1 = p.from_bytes(pixelFileName0);
	std::wcout << pixelFileName1 << std::endl;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		vertexFileName1.c_str(),    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		pixelFileName1.c_str(),    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//�e�����󂯂�{�[���ԍ�(4��)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//�{�[���̃X�L���E�F�C�g(4��)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = false;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 5;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8_UNORM;	//RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // t3 ���W�X�^

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV2;
	descRangeSRV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4); // t4 ���W�X�^

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV3;
	descRangeSRV3.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5); // t5 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[9];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(�X�L�j���O)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//�e�N�X�`�����W�X�^1��
	rootparams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootparams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�e�N�X�`�����W�X�^1��
	rootparams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparams[4].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootparams[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�萔�o�b�t�@(���C�g)
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`��2���ځj
	rootparams[6].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`��3���ځj
	rootparams[7].InitAsDescriptorTable(1, &descRangeSRV2, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`��3���ځj
	rootparams[8].InitAsDescriptorTable(1, &descRangeSRV3, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootsignature1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature1.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(pipelinestate1.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::PlayAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//0�ԃA�j���[�V�����擾
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	//�A�j���[�V�����̖��O�擾
	const char* animstackname = animstack->GetName();
	//�A�j���[�V�����̎��ԏ��
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	//�J�n���Ԏ擾
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ�����Ԃɂ���
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
		//�e�N�X�`��1���Ńe�N�X�`����ݒ肵�Ă�����
		if (textureData.shaderName != "null")
		{
			shaderName = textureData.shaderName;
			//�p�C�v���C������
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

