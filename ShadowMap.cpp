#include "ShadowMap.h"

ID3D12Device* ShadowMap::device = nullptr;

inline void ShadowMap::SetLightPos(const XMFLOAT3& lightPos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	//射影変換
	lightViewProjection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(45.0f),			//上下画角45度
		(float)window_width / window_height,//アスペクト比(画面横幅/画面立幅)
		0.1f, 1000.0f						//前端、奥端
	);

	//行列計算
	lightView = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&lightPos),
		DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
}

void ShadowMap::CreateBuffer()
{
    //HRESULT hr;

    //D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc{};
    //descriptor_heap_desc.NumDescriptors = 1;
    //descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    //descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    //descriptor_heap_desc.NodeMask = 0;
    //hr = device_->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(&dh_shadow_buffer_));

    //descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    //descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    //hr = device_->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(&dh_shadow_texture_));


    //D3D12_HEAP_PROPERTIES heap_properties{};
    //D3D12_RESOURCE_DESC resource_desc{};
    //D3D12_CLEAR_VALUE clear_value{};
    //heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
    //heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    //heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    //heap_properties.CreationNodeMask = 0;
    //heap_properties.VisibleNodeMask = 0;


    //resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    //resource_desc.Width = 1024;
    //resource_desc.Height = 1024;
    //resource_desc.DepthOrArraySize = 1;
    //resource_desc.MipLevels = 0;
    //resource_desc.Format = DXGI_FORMAT_R32_TYPELESS;
    //resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    //resource_desc.SampleDesc.Count = 1;
    //resource_desc.SampleDesc.Quality = 0;
    //resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    //clear_value.Format = DXGI_FORMAT_D32_FLOAT;
    //clear_value.DepthStencil.Depth = 1.0f;
    //clear_value.DepthStencil.Stencil = 0;

    //hr = device_->CreateCommittedResource(&heap_properties,
    //    D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, &clear_value, IID_PPV_ARGS(&shadow_buffer_));


    ////深度バッファのビューの作成
    //D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc{};
    //dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    //dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
    //dsv_desc.Texture2D.MipSlice = 0;
    //dsv_desc.Flags = D3D12_DSV_FLAG_NONE;
    //device_->CreateDepthStencilView(shadow_buffer_.Get(), &dsv_desc, dh_shadow_buffer_->GetCPUDescriptorHandleForHeapStart());



    //D3D12_SHADER_RESOURCE_VIEW_DESC resourct_view_desc{};
    //resourct_view_desc.Format = DXGI_FORMAT_R32_FLOAT;
    //resourct_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    //resourct_view_desc.Texture2D.MipLevels = 1;
    //resourct_view_desc.Texture2D.MostDetailedMip = 0;
    //resourct_view_desc.Texture2D.PlaneSlice = 0;
    //resourct_view_desc.Texture2D.ResourceMinLODClamp = 0.0F;
    //resourct_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    //device_->CreateShaderResourceView(shadow_buffer_.Get(), 
    //    &resourct_view_desc, dh_shadow_texture_->GetCPUDescriptorHandleForHeapStart());
}
