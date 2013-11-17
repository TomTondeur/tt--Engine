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

#include "../Helpers/D3DUtil.h"
#include "../Helpers/Namespace.h"

class RenderTarget2D
{
public:
	//Default constructor & destructor
	RenderTarget2D(void);
	RenderTarget2D(ID3D10Texture2D* pRTTexture, ID3D10Texture2D* pDepthTexture, ID3D10RenderTargetView* pRTView, ID3D10DepthStencilView* pDepthView, ID3D10ShaderResourceView* pColorMap, ID3D10ShaderResourceView* pDepthMap);
	virtual ~RenderTarget2D(void);

	//Methods
	void Create(unsigned int width, unsigned int height);
	void Create(ID3D10Texture2D* pRenderTargetBuffer);

	ID3D10RenderTargetView*& GetRenderTargetView(void);
	ID3D10DepthStencilView*& GetDepthStencilView(void);

	ID3D10ShaderResourceView* GetColorMap(void);
	ID3D10ShaderResourceView* GetDepthMap(void);
	
	ID3D10Texture2D* GetColorBuffer(void);
	ID3D10Texture2D* GetDepthBuffer(void);

private:
	//Datamembers
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10DepthStencilView* m_pDepthStencilView;
	
	ID3D10Texture2D* m_pColorTexture;
	ID3D10Texture2D* m_pDepthStencilTexture;
	
	ID3D10ShaderResourceView* m_pColorMapShaderResourceView;
	ID3D10ShaderResourceView* m_pDepthMapShaderResourceView;

	//Disabling default copy constructor & assignment operator
	RenderTarget2D(const RenderTarget2D& src);
	RenderTarget2D& operator=(const RenderTarget2D& src);
};
