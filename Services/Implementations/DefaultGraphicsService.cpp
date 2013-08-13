#include "DefaultGraphicsService.h"
#include "../../Graphics/GraphicsDevice.h"
#include "../../Graphics/Window.h"
#include "../../Graphics/Model3D.h"
#include "../../Graphics/Material.h"
#include "../../Graphics/EffectTechnique.h"
#include "../../Graphics/SpriteBatch.h"

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
}

Window* DefaultGraphicsService::GetWindow(void) const
{
	return m_pWindow;
}

SpriteBatch* DefaultGraphicsService::GetSpriteBatch(void) const
{
	return m_pSpriteBatch;
}