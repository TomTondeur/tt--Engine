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
