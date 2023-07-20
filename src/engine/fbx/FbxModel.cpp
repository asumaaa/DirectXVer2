#include "FbxModel.h"

TextureManager* FbxModel::textureManager = nullptr;
ID3D12Device* FbxModel::device = nullptr;

FbxModel::~FbxModel()
{
	//FBXシーンの解放
	fbxScene->Destroy();
}

void FbxModel::CreateBuffers()
{
	HRESULT result;

	//頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin)) * vertices.size();

	//頂点バッファ生成
	CD3DX12_HEAP_PROPERTIES v1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC v2 = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	result = device->CreateCommittedResource(
		&v1,
		D3D12_HEAP_FLAG_NONE,
		&v2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	//頂点バッファへのデータ転送
	VertexPosNormalUvSkin* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの生成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	//頂点インデックス全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	//インデックスバッファ生成
	v1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	v2 = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	result = device->CreateCommittedResource(
		&v1,
		D3D12_HEAP_FLAG_NONE,
		&v2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	////テクスチャの画像データ
	//const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);
	//assert(img);

	////リソース設定
	//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	metadata.format,
	//	metadata.width,
	//	(UINT)metadata.height,
	//	(UINT16)metadata.arraySize,
	//	(UINT16)metadata.mipLevels
	//);

	////テクスチャ用バッファの生成
	//v1 = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	//result = device->CreateCommittedResource(
	//	&v1,
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&texBuff));

	////テクスチャバッファにデータ転送
	//result = texBuff->WriteToSubresource(
	//	0,
	//	nullptr,
	//	img->pixels,
	//	(UINT)img->rowPitch,
	//	(UINT)img->slicePitch
	//	);

	////SRV用デスクリプタヒープを生成
	//D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	//descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//descHeapDesc.NumDescriptors = 2;
	//result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));

	////シェーダリソースビュー作成
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
	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { textureManager->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRVヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	//ハンドル1分のサイズ
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//テクスチャの番号に合わせてハンドルを進める
	if (textureNum > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum;
	}

	//SRVヒープの先頭にあるSRVをルートパラメータ1晩に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void FbxModel::DrawTexture1(ID3D12GraphicsCommandList* cmdList, int textureNum)
{
	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);

	////デスクリプタヒープのセット
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { textureManager->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRVヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	//ハンドル1分のサイズ
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//テクスチャの番号に合わせてハンドルを進める
	if (textureNum > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum;
	}

	//SRVヒープの先頭にあるSRVをルートパラメータ1晩に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
}

void FbxModel::DrawTexture2(ID3D12GraphicsCommandList* cmdList, int textureNum1, int textureNum2)
{
	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);

	////デスクリプタヒープのセット
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { textureManager->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRVヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	//ハンドル1分のサイズ
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//テクスチャ1枚目
	//テクスチャの番号に合わせてハンドルを進める
	if (textureNum1 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum1;
	}
	//SRVヒープの先頭にあるSRVをルートパラメータ1晩に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//テクスチャ2枚目
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum2 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum2;
	}
	//SRVヒープの先頭にあるSRVをルートパラメータ6番に設定
	cmdList->SetGraphicsRootDescriptorTable(6, srvGpuHandle);
}

void FbxModel::DrawTexture3(ID3D12GraphicsCommandList* cmdList, int textureNum1, int textureNum2, int textureNum3)
{
	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);

	////デスクリプタヒープのセット
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { textureManager->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRVヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	//ハンドル1分のサイズ
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//テクスチャ1枚目
	//テクスチャの番号に合わせてハンドルを進める
	if (textureNum1 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum1;
	}
	//SRVヒープの先頭にあるSRVをルートパラメータ1晩に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//テクスチャ2枚目
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum2 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum2;
	}
	//SRVヒープの先頭にあるSRVをルートパラメータ6番に設定
	cmdList->SetGraphicsRootDescriptorTable(6, srvGpuHandle);

	//テクスチャ3枚目
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum3 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum3;
	}
	//SRVヒープの先頭にあるSRVをルートパラメータ7番に設定
	cmdList->SetGraphicsRootDescriptorTable(7, srvGpuHandle);
}

void FbxModel::DrawTexture4(ID3D12GraphicsCommandList* cmdList, int textureNum1, int textureNum2, int textureNum3, int textureNum4)
{
	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);

	////デスクリプタヒープのセット
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { textureManager->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRVヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	//ハンドル1分のサイズ
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//テクスチャ1枚目
	//テクスチャの番号に合わせてハンドルを進める
	if (textureNum1 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum1;
	}
	//SRVヒープの先頭にあるSRVをルートパラメータ1晩に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//テクスチャ2枚目
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum2 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum2;
	}
	//SRVヒープの先頭にあるSRVをルートパラメータ6番に設定
	cmdList->SetGraphicsRootDescriptorTable(6, srvGpuHandle);

	//テクスチャ3枚目
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum3 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum3;
	}
	//SRVヒープの先頭にあるSRVをルートパラメータ7番に設定
	cmdList->SetGraphicsRootDescriptorTable(7, srvGpuHandle);

	//テクスチャ4枚目
	srvGpuHandle = textureManager->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	if (textureNum4 > 0)
	{
		srvGpuHandle.ptr += incrementSize * textureNum4;
	}
	//SRVヒープの先頭にあるSRVをルートパラメータ7番に設定
	cmdList->SetGraphicsRootDescriptorTable(8, srvGpuHandle);
}

void FbxModel::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	//描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
