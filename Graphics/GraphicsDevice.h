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

class RenderTarget2D;

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
	
	void ResetRenderTarget(void);
	void SetRenderTarget(RenderTarget2D* pRT);
	RenderTarget2D* GetRenderTarget(void) const;

	void EnableVSync(bool b);
	void ToggleVSync(void);
	void SetViewPort(const D3D10_VIEWPORT& vp); 

private:
	void CreateDeviceAndSwapChain(); 
	void CreateRenderTarget();
	void SetViewPort(); 

	//Datamembers
	ID3D10Device* m_pD3DDevice;
	IDXGISwapChain* m_pSwapChain; 
	RenderTarget2D* m_pRenderTarget;
	RenderTarget2D* m_pDefaultRenderTarget;
	
	tt::ViewportInfo m_MainViewportInfo;

	HWND m_hWindow;

	bool m_bUseVSync;

	//Disabling default copy constructor & assignment operator
	GraphicsDevice(const GraphicsDevice& src);
	GraphicsDevice& operator=(const GraphicsDevice& src);
};
