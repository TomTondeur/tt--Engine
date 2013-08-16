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

#include "RenderTarget2D.h"
#include "../Services/ServiceLocator.h"
#include "GraphicsDevice.h"

RenderTarget2D::RenderTarget2D(void) :	m_pRenderTargetView(nullptr),
										m_pDepthStencilView(nullptr),	
										m_pDepthStencilTexture(nullptr),	
										m_pColorMapShaderResourceView(nullptr),
										m_pDepthMapShaderResourceView(nullptr)
{

}

RenderTarget2D::~RenderTarget2D(void)
{
	m_pDepthStencilTexture->Release();
	m_pRenderTargetView->Release();
	m_pDepthStencilView->Release();
	m_pColorMapShaderResourceView->Release();
	m_pDepthMapShaderResourceView->Release();
}

//Methods

void RenderTarget2D::Create(unsigned int width, unsigned int height)
{
	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();

	//Create Texture2Ds

	D3D10_TEXTURE2D_DESC textureDesc;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;
	textureDesc.Usage = D3D10_USAGE_DEFAULT;

	ID3D10Texture2D* pRenderTargetTexture;
	HR( pD3DDevice->CreateTexture2D(&textureDesc, nullptr, &pRenderTargetTexture) );
	
	Create(pRenderTargetTexture);

	pRenderTargetTexture->Release();
}

void RenderTarget2D::Create(ID3D10Texture2D* pRenderTargetBuffer)
{
	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
	
	D3D10_TEXTURE2D_DESC rtBuff;
	pRenderTargetBuffer->GetDesc(&rtBuff);
	
	D3D10_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthStencilDesc.Width = rtBuff.Width;
	depthStencilDesc.Height = rtBuff.Height;
	depthStencilDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.Usage = D3D10_USAGE_DEFAULT;

	HR( pD3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilTexture) );

	//Create Views

	HR( pD3DDevice->CreateRenderTargetView(pRenderTargetBuffer, nullptr, &m_pRenderTargetView) );

	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR( pD3DDevice->CreateDepthStencilView(m_pDepthStencilTexture, &depthStencilViewDesc, &m_pDepthStencilView) );

	//Create Shader Resources

	HR( pD3DDevice->CreateShaderResourceView(pRenderTargetBuffer, nullptr, &m_pColorMapShaderResourceView) );
	
	D3D10_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	HR( pD3DDevice->CreateShaderResourceView(m_pDepthStencilTexture, &shaderResourceViewDesc, &m_pDepthMapShaderResourceView) );
}

ID3D10RenderTargetView*& RenderTarget2D::GetRenderTargetView(void)
{
	return m_pRenderTargetView;
}

ID3D10DepthStencilView*& RenderTarget2D::GetDepthStencilView(void)
{
	return m_pDepthStencilView;
}

ID3D10ShaderResourceView* RenderTarget2D::GetColorMap(void)
{
	return m_pColorMapShaderResourceView;
}

ID3D10ShaderResourceView* RenderTarget2D::GetDepthMap(void)
{
	return m_pDepthMapShaderResourceView;
}
