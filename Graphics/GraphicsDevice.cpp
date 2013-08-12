#include "GraphicsDevice.h"
#include "../Helpers/D3DUtil.h"
#include "../TTengine.h"
#include "../Services/ServiceLocator.h"

GraphicsDevice::GraphicsDevice(HWND windowHandle, unsigned short windowWith, unsigned short windowHeight)
						:m_pD3DDevice(nullptr)
						,m_pSwapChain(nullptr) 
						,m_pDepthStencilBuffer(nullptr) 
						,m_pDepthStencilView(nullptr)
						,m_pFont(nullptr)
						,m_pRenderTargetView(nullptr)
						,m_MainViewportInfo(windowWith,windowHeight)
						,m_hWindow(windowHandle)

{

}

GraphicsDevice::~GraphicsDevice(void)
{
    m_pRenderTargetView->Release();
    m_pDepthStencilBuffer->Release();
    m_pSwapChain->Release();
    m_pDepthStencilView->Release();
    m_pFont->Release();
    m_pD3DDevice->Release();
}

//Methods

void GraphicsDevice::Initialize(void)
{
    CreateDeviceAndSwapChain();
    CreateRenderTargetView();
    CreateDepthBufferAndView();
    SetViewPort();
    CreateDirect3DFont();
 
    m_pD3DDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}

void GraphicsDevice::Clear(void)
{
	m_pD3DDevice->ClearRenderTargetView(m_pRenderTargetView, D3DXCOLOR(1, 1, 1, 1));
	m_pD3DDevice->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0); 
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

void GraphicsDevice::DrawString(ID3DX10Font* pFont, const tstring &textRef, int xpos, int ypos) const
{
	const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
	RECT R = {xpos, ypos, 0, 0};
	pFont->DrawText(0, textRef.c_str(), -1, &R, DT_NOCLIP, BLACK);
}

ID3DX10Font* GraphicsDevice::CreateFontFromDescriptor(const D3DX10_FONT_DESC& fontDesc) const
{
	ID3DX10Font* pFont;
	
	_tcscpy_s((TCHAR*)fontDesc.FaceName, sizeof(_T("Arial")), _T("Arial"));
       
    D3DX10CreateFontIndirect(m_pD3DDevice, &fontDesc, &pFont);
 
    return pFont;
}

void GraphicsDevice::CreateDeviceAndSwapChain()
{
	//Init swapchain desc
	DXGI_SWAP_CHAIN_DESC swapChainDescStruct={};
	//Swapchain characteristics
	swapChainDescStruct.BufferDesc.Width					= m_MainViewportInfo.width;
	swapChainDescStruct.BufferDesc.Height					= m_MainViewportInfo.height;
	swapChainDescStruct.BufferDesc.RefreshRate.Numerator	= 60;
	swapChainDescStruct.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDescStruct.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescStruct.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescStruct.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	//Disable anti-aliasing
	swapChainDescStruct.SampleDesc.Count	= 1;
	swapChainDescStruct.SampleDesc.Quality	= 0;
	//Swapchain to window
	swapChainDescStruct.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescStruct.BufferCount = 1;
	swapChainDescStruct.OutputWindow = m_hWindow;
	swapChainDescStruct.Windowed = true;
	swapChainDescStruct.Flags = 0;

	UINT createDeviceFlags = 0;
	#ifndef NDEBUG
		createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
	#endif

	HR(D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, D3D10_SDK_VERSION, &swapChainDescStruct, &m_pSwapChain, &m_pD3DDevice));
}

void GraphicsDevice::CreateRenderTargetView()
{
	ID3D10Texture2D* backBuffer;
	HR(m_pSwapChain->GetBuffer(0,__uuidof(ID3D10Texture2D),reinterpret_cast<void**>(&backBuffer)));
	HR(m_pD3DDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView));
	backBuffer->Release();
}

void GraphicsDevice::CreateDepthBufferAndView()
{
	//Initialize depth/stencil description struct
	D3D10_TEXTURE2D_DESC depthStencilDesc={};
	depthStencilDesc.Width				= m_MainViewportInfo.width;
	depthStencilDesc.Height				= m_MainViewportInfo.height;
	depthStencilDesc.MipLevels			= 1;
	depthStencilDesc.ArraySize			= 1;
	depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count	= 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage				= D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags			= D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags		= 0;
	depthStencilDesc.MiscFlags			= 0;

	HR(m_pD3DDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer));
	HR(m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));
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

void GraphicsDevice::CreateDirect3DFont()
{
    D3DX10_FONT_DESC fontDesc;
    fontDesc.Height				= 24;
    fontDesc.Width				= 0;
    fontDesc.Weight				= 0;
    fontDesc.MipLevels			= 1;
    fontDesc.Italic				= false;
    fontDesc.CharSet			= DEFAULT_CHARSET;
    fontDesc.OutputPrecision	= OUT_DEFAULT_PRECIS;
    fontDesc.Quality			= DEFAULT_QUALITY;
    fontDesc.PitchAndFamily		= DEFAULT_PITCH | FF_DONTCARE;
    wcscpy_s(fontDesc.FaceName, sizeof(_T("Arial")), _T("Arial"));
       
    D3DX10CreateFontIndirect(m_pD3DDevice, &fontDesc, &m_pFont);
}
