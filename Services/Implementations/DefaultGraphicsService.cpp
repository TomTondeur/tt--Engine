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

DefaultGraphicsService::DefaultGraphicsService(void):m_pGraphicsDevice(nullptr)
													,m_pWindow(nullptr)
													,m_pSpriteBatch(nullptr)
{

}

DefaultGraphicsService::~DefaultGraphicsService(void)
{
	delete m_pGraphicsDevice;
	delete m_pWindow;
	delete m_pSpriteBatch;
	delete m_pSwapRT1;
	delete m_pSwapRT2;
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

ID3D10ShaderResourceView* DefaultGraphicsService::RenderPostProcessing(const tt::GameContext& context, std::multimap<unsigned int, PostProcessingEffect*, std::greater_equal<unsigned int> >& postProEffects)
{
	if(postProEffects.empty() )
		return nullptr;

	//First ppEffect: backbuffer as input, swap1 as output
	m_pGraphicsDevice->SetRenderTarget(m_pSwapRT1);
	m_pGraphicsDevice->Clear();
	postProEffects.begin()->second->Draw(context, m_pGraphicsDevice->GetRenderTarget() );

	//get iterator of second effect (will skip for_each if there is no second effect)
	auto itBegin = postProEffects.begin();
	itBegin++;

	for_each(itBegin, postProEffects.end(), [&](std::pair<unsigned int, PostProcessingEffect*>& effectPair)
	{
		//Additional ppEffects: swap1 as input, swap2 as output
		m_pGraphicsDevice->SetRenderTarget(m_pSwapRT2);
		m_pGraphicsDevice->Clear();
		effectPair.second->Draw(context, m_pSwapRT1);
		std::swap(m_pSwapRT1, m_pSwapRT2);
	});

	//swap2 is output, but they are swapped before returning => swap1 contains output (same if the for_each is skipped)
	return m_pSwapRT1->GetColorMap();
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

	m_pSwapRT1 = new RenderTarget2D(); m_pSwapRT1->Create(windowWidth, windowHeight);
	m_pSwapRT2 = new RenderTarget2D(); m_pSwapRT2->Create(windowWidth, windowHeight);
}

Window* DefaultGraphicsService::GetWindow(void) const
{
	return m_pWindow;
}

SpriteBatch* DefaultGraphicsService::GetSpriteBatch(void) const
{
	return m_pSpriteBatch;
}