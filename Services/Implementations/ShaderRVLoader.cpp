// Copyright © 2013 Tom Tondeur
// 
// This file is part of tt::Engine.
// 
// tt::Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// tt::Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with tt::Engine.  If not, see <http://www.gnu.org/licenses/>.

#include "../Interfaces/ResourceService.h"

#include "../ServiceLocator.h"
#include "../../Graphics/GraphicsDevice.h"

template<>
std::unique_ptr<ID3D10ShaderResourceView> ResourceService::LoadResource<ID3D10ShaderResourceView>(const std::tstring& assetFile)
{/*
	ID3D10ShaderResourceView* pTextureRV = nullptr;

	D3DX10_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory( &loadInfo, sizeof(D3DX10_IMAGE_LOAD_INFO) );
	loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_BC1_UNORM;

	HRESULT hr = D3DX10CreateShaderResourceViewFromFile( MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice(), assetFile.c_str(), &loadInfo, NULL, &pTextureRV, NULL);
	if(FAILED(hr))
		throw LoaderException(std::tstring(_T("texture ")) + assetFile);

	return std::unique_ptr<ID3D10ShaderResourceView>(pTextureRV);
	*/
	
	ID3D10ShaderResourceView* pTextureRV = nullptr;
	HR( D3DX10CreateShaderResourceViewFromFile( MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice(), assetFile.c_str(), NULL, NULL, &pTextureRV, NULL) );

	return std::unique_ptr<ID3D10ShaderResourceView>(pTextureRV);
}