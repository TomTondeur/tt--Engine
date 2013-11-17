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

#include "DefaultGraphicsService.h"
#include "../../Graphics/GraphicsDevice.h"
#include "../../Graphics/Window.h"
#include "../../Graphics/Model3D.h"
#include "../../Graphics/Material.h"
#include "../../Graphics/EffectTechnique.h"
#include "../../Graphics/SpriteBatch.h"
#include "../../Graphics/RenderTarget2D.h"
#include "../../Graphics/PostProcessingEffect.h"
#include "../../Components/SpriteComponent.h"

DefaultGraphicsService::DefaultGraphicsService(void):m_pGraphicsDevice(nullptr)
													,m_pWindow(nullptr)
													,m_pSpriteBatch(nullptr)
													,m_pSwapRT1(nullptr)
													,m_pSwapRT2(nullptr)
													,m_pPositionTexture(nullptr)
													,m_pPositionRT(nullptr)
													,m_pNormalTexture(nullptr)
													,m_pNormalRT(nullptr)
													,m_pDeferredDepthStencilView(nullptr)
{

}

DefaultGraphicsService::~DefaultGraphicsService(void)
{
	delete m_pGraphicsDevice;
	delete m_pWindow;
	delete m_pSpriteBatch;
	delete m_pSwapRT1;
	delete m_pSwapRT2;
	
	m_pPositionTexture->Release();
	m_pPositionRT->Release();
	m_pNormalTexture->Release();
	m_pNormalRT->Release();
	m_pDeferredDepthStencilView->Release();
}

//Methods

void DefaultGraphicsService::Draw(resource_ptr<Model3D> pModel, const tt::Matrix4x4& worldMat, resource_ptr<Material> pMat, const tt::GameContext& context)
{
	auto pD3DDevice = m_pGraphicsDevice->GetDevice();

	//Update shader variables
	pMat->Update(context, worldMat);

	// Set input layout	
	pD3DDevice->IASetInputLayout( pMat->GetInputLayout()->pInputLayout );

    // Set vertex buffer
    UINT offset = 0;
	auto &vertexDataInfo = pModel->GetVertexBufferInfo(pMat);

	pD3DDevice->IASetVertexBuffers(0, 1, &vertexDataInfo.pVertexBuffer , &vertexDataInfo.VertexStride, &offset);
   	
	// Set index buffer
	pD3DDevice->IASetIndexBuffer(pModel->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

    // Set primitive topology
    pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Apply technique & draw
	auto tech = pMat->GetActiveTechnique();
    D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);

    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        tech->GetPassByIndex(p)->Apply(0);
		pD3DDevice->DrawIndexed(pModel->GetNrOfIndices(), 0, 0); 
    }
}

void DefaultGraphicsService::DrawDeferred(resource_ptr<Model3D> pModel, const tt::Matrix4x4& worldMat, resource_ptr<Material> pMat, const tt::GameContext& context)
{
	auto pD3DDevice = m_pGraphicsDevice->GetDevice();

	//Get RT
	auto pOldRT = m_pGraphicsDevice->GetRenderTarget();

	//Clear G-Buffers
	float clearColor[] = {0.0f,0.0f,0.0f,0.0f};
	pD3DDevice->ClearRenderTargetView(m_pPositionRT, clearColor);
	pD3DDevice->ClearRenderTargetView(m_pNormalRT, clearColor);
	pD3DDevice->ClearDepthStencilView(m_pDeferredDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);

	//Set G-buffers
	ID3D10RenderTargetView* targets[] = {m_pPositionRT, m_pNormalRT};
	pD3DDevice->OMSetRenderTargets(2, targets, m_pDeferredDepthStencilView);

	// Set technique & draw
	pMat->SetActiveTechnique(_T("TechDeferred"));

	//Draw
	//Update shader variables
	pMat->Update(context, worldMat);

	// Set input layout	
	pD3DDevice->IASetInputLayout( pMat->GetInputLayout()->pInputLayout );

    // Set vertex buffer
    UINT offset = 0;
	auto &vertexDataInfo = pModel->GetVertexBufferInfo(pMat);

	pD3DDevice->IASetVertexBuffers(0, 1, &vertexDataInfo.pVertexBuffer , &vertexDataInfo.VertexStride, &offset);
   	
	// Set index buffer
	pD3DDevice->IASetIndexBuffer(pModel->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

    // Set primitive topology
    pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Apply technique & draw
	auto tech = pMat->GetActiveTechnique();
    D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);

    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        tech->GetPassByIndex(p)->Apply(0);
		pD3DDevice->DrawIndexed(pModel->GetNrOfIndices(), 0, 0); 
    }
	

	//Draw to file
	D3DX10SaveTextureToFile(m_pPositionTexture, D3DX10_IMAGE_FILE_FORMAT::D3DX10_IFF_JPG, L"TexPos.jpg");
	D3DX10SaveTextureToFile(m_pNormalTexture, D3DX10_IMAGE_FILE_FORMAT::D3DX10_IFF_JPG, L"TexNorm.jpg");

	//Restore RT & tech
	m_pGraphicsDevice->ResetRenderTarget();
	pMat->SetActiveTechnique(0);
}

Sprite DefaultGraphicsService::RenderPostProcessing(const tt::GameContext& context, std::multimap<unsigned int, PostProcessingEffect*, std::greater_equal<unsigned int> >& postProEffects)
{
	if(postProEffects.empty() ){
		Sprite backbufferSprite(tt::Matrix4x4::Identity );
		backbufferSprite.Color = tt::Vector4(1);
		backbufferSprite.pTexture = m_pGraphicsDevice->GetRenderTarget()->GetColorMap();
		return backbufferSprite;
	}

	//First ppEffect: backbuffer as input, swap1 as output
	auto pBackbufferRT = m_pGraphicsDevice->GetRenderTarget();
	m_pGraphicsDevice->SetRenderTarget(m_pSwapRT1);
	m_pGraphicsDevice->Clear();
	postProEffects.begin()->second->Draw(context,  pBackbufferRT);

	//get iterator of second effect (will skip for_each if there is no second effect)
	auto itBegin = postProEffects.begin();
	++itBegin;

	for_each(itBegin, postProEffects.end(), [&](std::pair<const unsigned int, PostProcessingEffect*>& effectPair)
	{
		//Additional ppEffects: swap1 as input, swap2 as output
		m_pGraphicsDevice->SetRenderTarget(m_pSwapRT2);
		m_pGraphicsDevice->Clear();
		effectPair.second->Draw(context, m_pSwapRT1);
		std::swap(m_pSwapRT1, m_pSwapRT2);
	});

	//Set RenderTarget back to default
	m_pGraphicsDevice->ResetRenderTarget();
	m_pGraphicsDevice->Clear();

	//swap2 is output, but they are swapped before returning => swap1 contains output (same if the for_each is skipped)
	Sprite postProSprite(tt::Matrix4x4::Identity );
	postProSprite.Color = tt::Vector4(1);
	postProSprite.pTexture = m_pSwapRT1->GetColorMap();
	
	return postProSprite;
}

GraphicsDevice* DefaultGraphicsService::GetGraphicsDevice(void) const
{
	return m_pGraphicsDevice;
}

void DefaultGraphicsService::InitWindow(int windowWidth, int windowHeight, TTengine* pEngine)
{
	m_pWindow = new Window(windowWidth, windowHeight);
	m_pWindow->Create(pEngine);
	m_pGraphicsDevice = new GraphicsDevice(m_pWindow->GetHandle(),windowWidth, windowHeight);
	m_pGraphicsDevice->Initialize();
	m_pSpriteBatch = new SpriteBatch();
	m_pSpriteBatch->Initialize();

	//Initialize post-processing swapchain
	m_pSwapRT1 = new RenderTarget2D(); m_pSwapRT1->Create(windowWidth, windowHeight);
	m_pSwapRT2 = new RenderTarget2D(); m_pSwapRT2->Create(windowWidth, windowHeight);

	//Initialize render targets used for deferred shading
	InitializeGBuffers();
}

void DefaultGraphicsService::InitializeGBuffers(void)
{
	D3D10_TEXTURE2D_DESC texDesc;
	texDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.Width = m_pGraphicsDevice->GetViewportInfo().width;
	texDesc.Height = m_pGraphicsDevice->GetViewportInfo().height;
	texDesc.ArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.CPUAccessFlags = NULL;
	texDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;
	
	m_pGraphicsDevice->GetDevice()->CreateTexture2D(&texDesc, nullptr, &m_pPositionTexture);
	m_pGraphicsDevice->GetDevice()->CreateRenderTargetView(m_pPositionTexture, nullptr, &m_pPositionRT);
	
	m_pGraphicsDevice->GetDevice()->CreateTexture2D(&texDesc, nullptr, &m_pNormalTexture);
	m_pGraphicsDevice->GetDevice()->CreateRenderTargetView(m_pNormalTexture, nullptr, &m_pNormalRT);

	//Depth-stencil view
	
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = NULL;

	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	ID3D10Texture2D* dsTex;
	m_pGraphicsDevice->GetDevice()->CreateTexture2D(&texDesc, nullptr, &dsTex);
	m_pGraphicsDevice->GetDevice()->CreateDepthStencilView(dsTex, &depthStencilViewDesc, &m_pDeferredDepthStencilView);
	dsTex->Release();

}

Window* DefaultGraphicsService::GetWindow(void) const
{
	return m_pWindow;
}

SpriteBatch* DefaultGraphicsService::GetSpriteBatch(void) const
{
	return m_pSpriteBatch;
}