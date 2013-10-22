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

ID3D10ShaderResourceView* LoadRAW(const std::tstring& assetFile);

template<>
std::unique_ptr<ID3D10ShaderResourceView> ResourceService::LoadResource<ID3D10ShaderResourceView>(const std::tstring& assetFile)
{
	ID3D10ShaderResourceView* pTextureRV = nullptr;
	std::tstring ext = assetFile.substr(assetFile.size() - 3, 3);
	if(ext == _T("raw"))
		return std::unique_ptr<ID3D10ShaderResourceView>(LoadRAW(assetFile));
	HR( D3DX10CreateShaderResourceViewFromFile( MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice(), assetFile.c_str(), NULL, NULL, &pTextureRV, NULL) );

	return std::unique_ptr<ID3D10ShaderResourceView>(pTextureRV);
}

//We assume the input value is an even power of 2 (2², 4², 8², 16², 32², 64², 128², 256², 512², 1024²)
//For these numbers, sqrt is the same as halving the number of trailing zeroes
inline unsigned short FastSqrt(unsigned int v)
{
	//Get trailing zeroes
	unsigned char c = 32; // c will be the number of zero bits on the right
	if (v) c--;
	if (v & 0x0000FFFF) c -= 16;
	if (v & 0x00FF00FF) c -= 8;
	if (v & 0x0F0F0F0F) c -= 4;
	if (v & 0x33333333) c -= 2;
	if (v & 0x55555555) c -= 1;

	//Return sqrt
	return v >> (c >> 1);
}

//Read .RAW file, textures must me square and dimensions must be a power 2
ID3D10ShaderResourceView* LoadRAW(const std::tstring& assetFile)
{
	//READ BINARY FILE

	//open binary file 
	std::ifstream inFile;
	inFile.open(assetFile.c_str(),ios::binary | ios::ate); //Open file at the end so we can get filesize

	//Failed to load file
	if (!inFile)
		throw LoaderException(std::tstring(_T("texture ")) + assetFile);

	auto filesize = inFile.tellg();
	unsigned short texSize = FastSqrt(static_cast<unsigned int>(filesize) >> 1);

	//Jump back to beginning
	inFile.seekg(0, ios::beg);
	
	//read height info	
	unsigned short* heightValues = new unsigned short[static_cast<unsigned int>(filesize >> 1)];
	inFile.read(reinterpret_cast<char*>(heightValues), filesize);
	inFile.close();
	
	//BUILD DX TEXTURE
	
	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
	ID3D10Texture2D* pTexture2D = nullptr;
	ID3D10ShaderResourceView* pShaderRV = nullptr;
		
	D3D10_SUBRESOURCE_DATA initData;
	initData.pSysMem = heightValues;
	initData.SysMemPitch = texSize*2;
	
	//Crate the texture
	D3D10_TEXTURE2D_DESC texDesc;
    texDesc.Height = texDesc.Width = texSize;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R16_UINT;
    texDesc.Usage = D3D10_USAGE_DEFAULT;
    texDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;

	HR(pD3DDevice->CreateTexture2D(&texDesc, &initData, &pTexture2D));
	
    // Create the resource view
	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));
    SRVDesc.Format = texDesc.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = texDesc.MipLevels;

	HR(pD3DDevice->CreateShaderResourceView(pTexture2D, &SRVDesc, &pShaderRV));

	//Cleanup
	delete[] heightValues;
	//pTexture2D->Release();

	return pShaderRV;
}