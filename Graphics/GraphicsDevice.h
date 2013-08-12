#pragma once

#include "../Helpers/Namespace.h"

class GraphicsDevice
{
public:
	//Default constructor & destructor
	GraphicsDevice(HWND windowHandle, unsigned short windowWith, unsigned short windowHeight);
	virtual ~GraphicsDevice(void);

	//Methods
	void Initialize(void);
	ID3D10Device* GetDevice(void);
	const tt::ViewportInfo& GetViewportInfo(void) const;

	void Clear(void);
	void Present(void);
	
	void DrawString(ID3DX10Font* pFont, const std::tstring &textRef, int xpos, int ypos) const;
	ID3DX10Font* CreateFontFromDescriptor(const D3DX10_FONT_DESC& fontDesc) const;

	void EnableVSync(bool b);

private:
	void CreateDeviceAndSwapChain(); 
	void CreateRenderTargetView();
	void CreateDepthBufferAndView(); 
	void SetViewPort(); 

	void CreateDirect3DFont(); 
	
	void DiscardResources();

	//Datamembers
	ID3D10Device* m_pD3DDevice;
	IDXGISwapChain* m_pSwapChain; 
	ID3D10Texture2D* m_pDepthStencilBuffer; 
	ID3D10DepthStencilView* m_pDepthStencilView; 
	ID3DX10Font* m_pFont;
	ID3D10RenderTargetView* m_pRenderTargetView; 
	
	tt::ViewportInfo m_MainViewportInfo;

	HWND m_hWindow;

	bool m_bUseVSync;

	//Disabling default copy constructor & assignment operator
	GraphicsDevice(const GraphicsDevice& src);
	GraphicsDevice& operator=(const GraphicsDevice& src);
};
