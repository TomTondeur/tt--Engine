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

#include "GraphicsDevice.h"
#include "../Helpers/D3DUtil.h"
#include "../TTengine.h"
#include "../Services/ServiceLocator.h"
#include "RenderTarget2D.h"

GraphicsDevice::GraphicsDevice(HWND windowHandle, unsigned short windowWith, unsigned short windowHeight)
						:m_pD3DDevice(nullptr)
						,m_pSwapChain(nullptr) 
						,m_pRenderTarget(nullptr)
						,m_pDefaultRenderTarget(nullptr)
						,m_MainViewportInfo(windowWith,windowHeight)
						,m_hWindow(windowHandle)

{

}

GraphicsDevice::~GraphicsDevice(void)
{
    delete m_pDefaultRenderTarget;
    m_pSwapChain->Release();
    m_pD3DDevice->Release();
}

//Methods

void GraphicsDevice::Initialize(void)
{
    CreateDeviceAndSwapChain();
    CreateRenderTarget();
    SetViewPort();
 
    m_pD3DDevice->OMSetRenderTargets(1, &m_pRenderTarget->GetRenderTargetView(), m_pRenderTarget->GetDepthStencilView() );
}

void GraphicsDevice::Clear(void)
{
	m_pD3DDevice->ClearRenderTargetView(m_pRenderTarget->GetRenderTargetView(), D3DXCOLOR(0, 0, 0, 1));
	m_pD3DDevice->ClearDepthStencilView(m_pRenderTarget->GetDepthStencilView(), D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0); 
	m_pD3DDevice->OMSetDepthStencilState(0,0);
}

void GraphicsDevice::Present(void)
{
	HR(m_pSwapChain->Present(m_bUseVSync? 1 : 0,0));
}

void GraphicsDevice::EnableVSync(bool b)
{
	m_bUseVSync = b;
}

ID3D10Device* GraphicsDevice::GetDevice(void)
{
	return m_pD3DDevice;
}

const tt::ViewportInfo& GraphicsDevice::GetViewportInfo(void) const
{
	return m_MainViewportInfo;
}

void GraphicsDevice::ResetRenderTarget(void)
{
	m_pD3DDevice->OMSetRenderTargets( 1, &m_pDefaultRenderTarget->GetRenderTargetView(), m_pDefaultRenderTarget->GetDepthStencilView() );
	m_pRenderTarget = m_pDefaultRenderTarget;
}

void GraphicsDevice::SetRenderTarget(RenderTarget2D* pRT)
{
	m_pD3DDevice->OMSetRenderTargets(1, &pRT->GetRenderTargetView(), pRT->GetDepthStencilView() );
	m_pRenderTarget = pRT;
}

RenderTarget2D* GraphicsDevice::GetRenderTarget(void) const
{
	return m_pRenderTarget;
}

void GraphicsDevice::CreateDeviceAndSwapChain()
{
	//Init swapchain desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc={};
	//Swapchain characteristics
	swapChainDesc.BufferDesc.Width					= m_MainViewportInfo.width;
	swapChainDesc.BufferDesc.Height					= m_MainViewportInfo.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator	= 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	//Disable anti-aliasing
	swapChainDesc.SampleDesc.Count	= 1;
	swapChainDesc.SampleDesc.Quality	= 0;
	//Swapchain to window
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_hWindow;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT createDeviceFlags = 0;
	#ifndef NDEBUG
		createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
	#endif

	HR(D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, D3D10_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pD3DDevice));
}

void GraphicsDevice::CreateRenderTarget()
{
	ID3D10Texture2D *pBackbuffer = nullptr;
	HR( m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&pBackbuffer)) );

	m_pRenderTarget = m_pDefaultRenderTarget = new RenderTarget2D();
	m_pRenderTarget->Create(pBackbuffer);

	pBackbuffer->Release();
}

void GraphicsDevice::SetViewPort()
{
	//Initialize viewport

	D3D10_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = m_MainViewportInfo.width;
	vp.Height = m_MainViewportInfo.height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	m_pD3DDevice->RSSetViewports(1, &vp);
}
