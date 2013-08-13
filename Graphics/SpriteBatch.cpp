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

#include "SpriteBatch.h"
#include "../Services/ServiceLocator.h"
#include "../Components/SpriteComponent.h"
#include "GraphicsDevice.h"
#include "Materials/SpriteMaterial.h"
#include "EffectTechnique.h"

struct SpriteVertex{
	SpriteVertex(const tt::Matrix4x4& transform, tt::Vector4 color):color(static_cast<D3DXCOLOR>(color))
																	,matRow0(transform._11, transform._12, transform._13)
																	,matRow1(transform._21, transform._22, transform._23)
																	,matRow2(transform._31, transform._32, transform._33)
																	,pos(transform._41, transform._42, transform._43)
	{}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 matRow0;
	D3DXVECTOR3 matRow1;
	D3DXVECTOR3 matRow2;
	D3DXCOLOR color;
};

SpriteBatch::SpriteBatch(void):m_pMaterial(nullptr),m_pVertexBuffer(nullptr),m_VertexBufferStride(0),m_NrOfSprites(0)
{

}

SpriteBatch::~SpriteBatch(void)
{

}

//Methods

void SpriteBatch::Initialize(void)
{
	m_pMaterial = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<SpriteMaterial>(_T("DefaultSpriteMaterial"));

	for(auto& ilElem : m_pMaterial->GetInputLayout()->InputLayoutDesc)
			m_VertexBufferStride += ilElem.Offset;

	//VERTEX BUFFER
	D3D10_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = m_VertexBufferStride * sc_MaxNrOfSprites;
	bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer);
}

void SpriteBatch::Draw(const Sprite& sprite)
{
	if(m_NrOfSprites++ > sc_MaxNrOfSprites)
		throw exception();

	auto it = m_Sprites.find( sprite.pTexture.get() );

	if(it == m_Sprites.end() )
		it = m_Sprites.insert( make_pair(sprite.pTexture.get(), std::vector<SpriteVertex>() ) ).first;
	
	it->second.push_back(SpriteVertex(sprite.Transform, sprite.Color) );
}

void SpriteBatch::Flush(const tt::GameContext& context)
{
	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
	
	//Set View Transform
	D3D10_VIEWPORT viewport;
	UINT viewportCount = 1;
	pD3DDevice->RSGetViewports(&viewportCount, &viewport);

	float scaleX = (viewport.Width>0) ? 2.0f/viewport.Width :0;
	float scaleY = (viewport.Height>0)? 2.0f/viewport.Height:0;

	tt::Matrix4x4 viewTransform(scaleX, 0			, 0, 0
								,0		, -scaleY	, 0, 0
								,0		, 0			, 1, 0
								,-1		, 1			, 0, 1);
								  
	m_pMaterial->SetVariable(_T("ViewTransform"), viewTransform);

	//Prepare Input Assembler
	unsigned int offset=0;
	pD3DDevice->IASetInputLayout(m_pMaterial->GetInputLayout()->pInputLayout);
	pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	pD3DDevice->IASetVertexBuffers(0, 0, &m_pVertexBuffer, &m_VertexBufferStride, &offset);
	
	//Map Vertexbuffer
	SpriteVertex* pVertices;
	int bufferPosition = -1;
	m_pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&pVertices);

	unsigned int idx=0;
	for(auto& _pair : m_Sprites)
		for(auto& spriteVert : _pair.second)
			pVertices[idx++] = spriteVert;

	m_pVertexBuffer->Unmap();
	
	//Start drawing
	unsigned int batchStart=0;
	for(auto& _pair : m_Sprites){
		//Set texture
		m_pMaterial->SetVariable(_T("Texture"), _pair.first);
		
		ID3D10Resource* pResrc;
		ID3D10Texture2D* pTexture;
		D3D10_TEXTURE2D_DESC desc;

		_pair.first->GetResource(&pResrc);
		pTexture  = static_cast<ID3D10Texture2D*>(pResrc);
		pTexture->GetDesc(&desc);

		//Set texture dimensions
		m_pMaterial->SetVariable(_T("Width"), (int)desc.Width);
		m_pMaterial->SetVariable(_T("Height"), (int)desc.Height);
			
		//Draw
		D3D10_TECHNIQUE_DESC techDesc;
		m_pMaterial->GetActiveTechnique()->GetDesc( &techDesc );
		for(UINT p = 0; p < techDesc.Passes; ++p){
			m_pMaterial->GetActiveTechnique()->GetPassByIndex(p)->Apply(0);
			pD3DDevice->Draw(_pair.second.size(), batchStart); 
		}

		//Set new batch start index
		batchStart += _pair.second.size();
	}

	m_Sprites.clear();
	m_NrOfSprites = 0;
}
