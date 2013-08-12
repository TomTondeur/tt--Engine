#include "../Interfaces/ResourceService.h"

#include "../ServiceLocator.h"
#include "../../Graphics/GraphicsDevice.h"

template<>
std::unique_ptr<ID3D10ShaderResourceView> ResourceService::LoadResource<ID3D10ShaderResourceView>(std::tstring assetFile)
{
	ID3D10ShaderResourceView* pTextureRV = nullptr;

	D3DX10_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory( &loadInfo, sizeof(D3DX10_IMAGE_LOAD_INFO) );
	loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_BC1_UNORM;

	HRESULT hr = D3DX10CreateShaderResourceViewFromFile( MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice(), assetFile.c_str(), &loadInfo, NULL, &pTextureRV, NULL);
	if(FAILED(hr))
		throw LoaderException(std::tstring(_T("texture ")) + assetFile);

	return std::unique_ptr<ID3D10ShaderResourceView>(pTextureRV);
}