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

#include "ParticleEmitterComponent.h"
#include "../Graphics/Material.h"
#include "../Graphics/EffectTechnique.h"
#include "../Graphics/GraphicsDevice.h"
#include "../Graphics/RenderTarget2D.h"
#include "../Services/ServiceLocator.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

using namespace std;

struct ParticleVertex{
	ParticleVertex(D3DXVECTOR3 pos, D3DXVECTOR3 vel, float timer, unsigned int type):Position(pos),
																					Velocity(vel),
																					Timer(timer),
																					Type(type){}
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Velocity;
	float Timer;
	unsigned int Type;
};

unique_ptr<RenderTarget2D> ParticleEmitterComponent::s_pRenderTarget = nullptr;
vector<ParticleEmitterComponent*> ParticleEmitterComponent::s_DeferredParticles = vector<ParticleEmitterComponent*>();

ParticleEmitterComponent::ParticleEmitterComponent(resource_ptr<Material> pMat, const TransformComponent* pTransform, unsigned int nrOfParticles):m_pInitVB(nullptr),
																																					m_pUpdateVB(nullptr),
																																					m_pDrawVB(nullptr),
																																					m_pMaterial(pMat),
																																					m_VertexStride(0),
																																					m_NrOfParticles(nrOfParticles),
																																					m_pTransform(pTransform)
{

}

ParticleEmitterComponent::~ParticleEmitterComponent(void)
{
	m_pInitVB->Release();
	m_pUpdateVB->Release();
	m_pDrawVB->Release();
}

//Methods

void ParticleEmitterComponent::Initialize(void)
{
	for(auto& elem : m_pMaterial->GetInputLayout()->InputLayoutDesc)
		m_VertexStride += elem.Offset;
	
	//Create vertex buffers
	CreateVertexBuffers();

	//Create RT
	if(s_pRenderTarget == nullptr){
		auto vpInfo = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetViewportInfo();
		s_pRenderTarget = unique_ptr<RenderTarget2D>(new RenderTarget2D() );
		s_pRenderTarget->Create(vpInfo.width, vpInfo.height);
	}

	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
    UINT offset = 0;

	//Set update technique
	m_pMaterial->SetActiveTechnique(_T("UpdateParticles") );
	m_pMaterial->InitializeEffectVariables();

	// Configure Input Assembler
	pD3DDevice->IASetInputLayout( m_pMaterial->GetInputLayout()->pInputLayout );
	pD3DDevice->IASetVertexBuffers(0, 1, &m_pInitVB, &m_VertexStride, &offset);
    pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	//The updated vertices are streamed out to the target VB
	pD3DDevice->SOSetTargets(1, &m_pUpdateVB, &offset);

	// Apply technique & draw
	auto tech = m_pMaterial->GetActiveTechnique();
    D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);

    for(UINT p = 0; p < techDesc.Passes; ++p){
        tech->GetPassByIndex(p)->Apply(0);
		pD3DDevice->Draw(1,0); 
    }
	
	//Empty SO target
	ID3D10Buffer* bufferArray[1] = {0};
	pD3DDevice->SOSetTargets(1, bufferArray, &offset);
}

void ParticleEmitterComponent::Draw(const tt::GameContext& context)
{
	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
    UINT offset = 0;

	//Update shader variables
	m_pMaterial->SetActiveTechnique(_T("UpdateParticles") );	
	m_pMaterial->Update(context, m_pTransform->GetWorldMatrix() );

	// Configure Input Assembler
	pD3DDevice->IASetInputLayout( m_pMaterial->GetInputLayout()->pInputLayout );
	pD3DDevice->IASetVertexBuffers(0, 1, &m_pUpdateVB, &m_VertexStride, &offset);
    pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	//The updated vertices are streamed out to the target VB
	pD3DDevice->SOSetTargets(1, &m_pDrawVB, &offset);

	// Apply technique & draw
	auto tech = m_pMaterial->GetActiveTechnique();
    D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);

    for(UINT p = 0; p < techDesc.Passes; ++p){
        tech->GetPassByIndex(p)->Apply(0);
		pD3DDevice->DrawAuto(); 
    }
	
	//Empty SO target
	ID3D10Buffer* bufferArray[1] = {0};
	pD3DDevice->SOSetTargets(1, bufferArray, &offset);

	s_DeferredParticles.push_back(this);
	
	//Put latest vertices back in UpdateVB, to use as input in next update
	std::swap(m_pUpdateVB, m_pDrawVB);
}

Sprite ParticleEmitterComponent::RenderDeferred(const tt::GameContext& context)
{
	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
	
	//MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->SetRenderTarget(s_pRenderTarget.get() );
	//MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->Clear();
	
	for(auto pEmitter : s_DeferredParticles){	
		//Update shader variables
		pEmitter->m_pMaterial->SetActiveTechnique(_T("RenderParticles") );	
		pEmitter->m_pMaterial->Update(context, pEmitter->m_pTransform->GetWorldMatrix() );

		// Configure Input Assembler
		pD3DDevice->IASetInputLayout( pEmitter->m_pMaterial->GetInputLayout()->pInputLayout );
		UINT offset = 0;
		pD3DDevice->IASetVertexBuffers(0, 1, &pEmitter->m_pDrawVB, &pEmitter->m_VertexStride, &offset);
		pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	
		// Apply technique & draw
		auto tech = pEmitter->m_pMaterial->GetActiveTechnique();
		D3D10_TECHNIQUE_DESC techDesc;
		tech->GetDesc(&techDesc);

		for(UINT p = 0; p < techDesc.Passes; ++p){
			tech->GetPassByIndex(p)->Apply(0);
			pD3DDevice->DrawAuto(); 
		}
	}

	//MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->ResetRenderTarget();
	s_DeferredParticles.clear();

	Sprite oSprite(tt::Matrix4x4::Identity);
	oSprite.pTexture = s_pRenderTarget->GetColorMap();
	oSprite.Color = tt::Vector4(1);

	return oSprite;
}

void ParticleEmitterComponent::CreateVertexBuffers(void)
{
	//Create vertex buffer for initial mesh data
	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();

	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_DEFAULT;
	vbd.ByteWidth = m_VertexStride; //Init VB only holds 1 vertex
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	
	ParticleVertex initVert[1] = {ParticleVertex(D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,40,0), 0, 0)};
	D3D10_SUBRESOURCE_DATA initData;
	initData.pSysMem = initVert;
	HR( pD3DDevice->CreateBuffer(&vbd, &initData, &m_pInitVB) );

	//Create empty update & draw buffers
	vbd.Usage = D3D10_USAGE_DEFAULT;
	vbd.ByteWidth = m_NrOfParticles * m_VertexStride;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER | D3D10_BIND_STREAM_OUTPUT;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	
	HR( pD3DDevice->CreateBuffer(&vbd, nullptr, &m_pUpdateVB) );
	HR( pD3DDevice->CreateBuffer(&vbd, nullptr, &m_pDrawVB) );
}
