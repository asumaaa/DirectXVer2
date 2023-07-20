#include "FbxModel.h"

TextureManager* FbxModel::textureManager = nullptr;
ID3D12Device* FbxModel::device = nullptr;

FbxModel::~FbxModel()
{
	//FBX�V�[���̉��
	fbxScene->Destroy();
}

void FbxModel::CreateBuffers()
{
	HRESULT result;

	//���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin)) * vertices.size();

	//���_�o�b�t�@����
	CD3DX12_HEAP_PROPERTIES v1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC v2 = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	result = device->CreateCommittedResource(
		&v1,
		D3D12_HEAP_FLAG_NONE,
		&v2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUvSkin* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̐���
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	//���_�C���f�b�N�X�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	//�C���f�b�N�X�o�b�t�@����
	v1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	v2 = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	result = device->CreateCommittedResource(
		&v1,
		D3D12_HEAP_FLAG_NONE,
		&v2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	////�e�N�X�`���̉摜�f�[�^
	//const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);
	//assert(img);

	////���\�[�X�ݒ�
	//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	metadata.format,
	//	metadata.width,
	//	(UINT)metadata.height,
	//	(UINT16)metadata.arraySize,
	//	(UINT16)metadata.mipLevels
	//);

	////�e�N�X�`���p�o�b�t�@�̐���
	//v1 = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	//result = device->CreateCommittedResource(
	//	&v1,
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&texBuff));

	////�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//result = texBuff->WriteToSubresource(
	//	0,
	//	nullptr,
	//	img->pixels,
	//	(UINT)img->rowPitch,
	//	(UINT)img->slicePitch
	//	);

	////SRV�p�f�X�N���v�^�q�[�v�𐶐�
	//D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	//descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//descHeapDesc.NumDescriptors = 2;
	//result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));

	////�V�F�[�_���\�[�X�r���[�쐬
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();
	//srvDesc.Format = resDesc.Format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MipLevels = 1;
	//device->CreateShaderResourceView(
	//	texBuff.Get(),
	//	&srvDesc,
	//	descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	//);
}

void FbxModel::Draw(ID3D12GraphicsCommandList* cmdList, int textureNum)
{
	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@���Z�b�g
	cmdList->IASetIndexBuffer(&ibView);

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { textureManager->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	//�n���h��1���̃T�C�Y
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�e�N�X�`���̔ԍ��ɍ��킹�ăn���h����i�߂�
	if (textureNum > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum;
	}

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�ӂɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//�`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void FbxModel::DrawTexture1(ID3D12GraphicsCommandList* cmdList, int textureNum)
{
	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@���Z�b�g
	cmdList->IASetIndexBuffer(&ibView);

	////�f�X�N���v�^�q�[�v�̃Z�b�g
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { textureManager->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	//�n���h��1���̃T�C�Y
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�e�N�X�`���̔ԍ��ɍ��킹�ăn���h����i�߂�
	if (textureNum > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum;
	}

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�ӂɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
}

void FbxModel::DrawTexture2(ID3D12GraphicsCommandList* cmdList, int textureNum1, int textureNum2)
{
	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@���Z�b�g
	cmdList->IASetIndexBuffer(&ibView);

	////�f�X�N���v�^�q�[�v�̃Z�b�g
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { textureManager->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	//�n���h��1���̃T�C�Y
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�e�N�X�`��1����
	//�e�N�X�`���̔ԍ��ɍ��킹�ăn���h����i�߂�
	if (textureNum1 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum1;
	}
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�ӂɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//�e�N�X�`��2����
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum2 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum2;
	}
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^6�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(6, srvGpuHandle);
}

void FbxModel::DrawTexture3(ID3D12GraphicsCommandList* cmdList, int textureNum1, int textureNum2, int textureNum3)
{
	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@���Z�b�g
	cmdList->IASetIndexBuffer(&ibView);

	////�f�X�N���v�^�q�[�v�̃Z�b�g
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { textureManager->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	//�n���h��1���̃T�C�Y
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�e�N�X�`��1����
	//�e�N�X�`���̔ԍ��ɍ��킹�ăn���h����i�߂�
	if (textureNum1 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum1;
	}
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�ӂɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//�e�N�X�`��2����
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum2 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum2;
	}
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^6�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(6, srvGpuHandle);

	//�e�N�X�`��3����
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum3 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum3;
	}
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^7�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(7, srvGpuHandle);
}

void FbxModel::DrawTexture4(ID3D12GraphicsCommandList* cmdList, int textureNum1, int textureNum2, int textureNum3, int textureNum4)
{
	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@���Z�b�g
	cmdList->IASetIndexBuffer(&ibView);

	////�f�X�N���v�^�q�[�v�̃Z�b�g
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { textureManager->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	//�n���h��1���̃T�C�Y
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�e�N�X�`��1����
	//�e�N�X�`���̔ԍ��ɍ��킹�ăn���h����i�߂�
	if (textureNum1 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum1;
	}
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�ӂɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//�e�N�X�`��2����
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum2 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum2;
	}
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^6�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(6, srvGpuHandle);

	//�e�N�X�`��3����
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum3 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum3;
	}
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^7�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(7, srvGpuHandle);

	//�e�N�X�`��4����
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum4 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum4;
	}
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^7�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(8, srvGpuHandle);
}

void FbxModel::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	//�`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
