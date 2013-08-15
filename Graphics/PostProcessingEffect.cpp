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

#include "PostProcessingEffect.h"

#include "../Services/ServiceLocator.h"
#include "GraphicsDevice.h"
#include "Materials/PostProcessingMaterial.h"
#include "EffectTechnique.h"

struct PostProcessingVertex
{
	PostProcessingVertex(D3DXVECTOR3 pos, D3DXVECTOR2 texCoord):Position(pos), TexCoord(texCoord) {}

	D3DXVECTOR3 Position;
	D3DXVECTOR2 TexCoord;
};

ID3D10Buffer* PostProcessingEffect::s_pVertexBuffer = nullptr;
unsigned int PostProcessingEffect::s_VertexBufferStride = 3*4 + 2*4; //NrOfVertices * (PosFloatComponents * 4 bytes + TexCoordFloatComponents * 4 bytes)

PostProcessingEffect::PostProcessingEffect(resource_ptr<PostProcessingMaterial> pMaterial):m_pMaterial(pMaterial)
{

}

PostProcessingEffect::~PostProcessingEffect(void)
{

}

//Methods

void PostProcessingEffect::Initialize(void)
{
	//Check if InputLayout is valid
	auto ilDesc = m_pMaterial->GetInputLayout()->InputLayoutDesc;

	if(ilDesc.size() != 2 
		|| ilDesc[0].Semantic != InputLayoutSemantic::Position
		|| ilDesc[0].Format != DXGI_FORMAT_R32G32B32_FLOAT
		|| ilDesc[1].Semantic != InputLayoutSemantic::TexCoord
		|| ilDesc[1].Format != DXGI_FORMAT_R32G32_FLOAT)
		throw exception();

	if(s_pVertexBuffer == nullptr){//WRONG -> STATIC VB
		//Build vertex buffer	
		D3D10_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D10_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = s_VertexBufferStride * 4; //4 vertices
		bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		PostProcessingVertex pVertices[] = {PostProcessingVertex(D3DXVECTOR3(-1,-1,0), D3DXVECTOR2(0,0) ),
											PostProcessingVertex(D3DXVECTOR3( 1,-1,0), D3DXVECTOR2(1,0) ),
											PostProcessingVertex(D3DXVECTOR3(-1, 1,0), D3DXVECTOR2(0,1) ),
											PostProcessingVertex(D3DXVECTOR3( 1, 1,0), D3DXVECTOR2(1,1) )};										

		D3D10_SUBRESOURCE_DATA initData;
		initData.pSysMem = pVertices;

		HR( MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice()->CreateBuffer(&bufferDesc, &initData, &s_pVertexBuffer) );
	}
}

void PostProcessingEffect::Draw(const tt::GameContext& context, RenderTarget2D* pInputRT)
{
	m_pMaterial->UpdateEffectVariables(context); //Pass pInputRT (SetVariable?)

	//Drawcall
	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();

	pD3DDevice->IASetInputLayout(m_pMaterial->GetInputLayout()->pInputLayout);
	pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	unsigned int offset=0;
	pD3DDevice->IASetVertexBuffers(0, 1, &s_pVertexBuffer, &s_VertexBufferStride, &offset);

	D3D10_TECHNIQUE_DESC techDesc;
	m_pMaterial->GetActiveTechnique()->GetDesc(&techDesc);
	for(unsigned int p=0; p < techDesc.Passes; ++p){
		m_pMaterial->GetActiveTechnique()->GetPassByIndex(p)->Apply(0);
		pD3DDevice->Draw(4, 0);
	}
}
