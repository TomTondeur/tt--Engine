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

#include "SpriteFont.h"
#include "Materials/TextMaterial.h"
#include "../Services/ServiceLocator.h"
#include "GraphicsDevice.h"
#include "EffectTechnique.h"
#include "SpriteBatch.h"

resource_ptr<TextMaterial> SpriteFont::s_pMaterial = resource_ptr<TextMaterial>();
ID3D10Buffer* SpriteFont::s_pVertexBuffer = nullptr;
unsigned int  SpriteFont::s_VertexBufferStride = 0;

TextData::TextData(const std::tstring& text, const tt::Vector2& pos, const tt::Vector4& color):Text( text.begin(), text.end() ),Position(pos),Color(color)
{}

struct TextVertex
{
	TextVertex(tt::Vector2 pos, tt::Vector2 texCoord, tt::Vector2 charSize, tt::Vector4 color, unsigned int channel) :	 Position(D3DXVECTOR3(pos.x, pos.y, .0f))
																														,TexCoord(static_cast<D3DXVECTOR2>(texCoord))
																														,CharSize(static_cast<D3DXVECTOR2>(charSize))
																														,Color	 (static_cast<D3DXCOLOR>(color))
																														,Channel(channel) {}

	D3DXVECTOR3 Position;
	D3DXCOLOR Color;
	D3DXVECTOR2 TexCoord;
	D3DXVECTOR2 CharSize;
	unsigned int Channel;
};

SpriteFont::SpriteFont(void):m_FontSize(0), m_NrOfCharsToDraw(0)
{}

SpriteFont::~SpriteFont(void)
{}

//Methods

void SpriteFont::Initialize()
{
	//initialize the necessary material, vertexbuffer... to draw text

	s_pMaterial = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<TextMaterial>(_T("DefaultTextMaterial"));

	for(auto& ilElem : s_pMaterial->GetInputLayout()->InputLayoutDesc)
		s_VertexBufferStride += ilElem.Offset;
		
	//VERTEX BUFFER
	D3D10_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = s_VertexBufferStride * sc_MaxNrOfChars;
	bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	HR(MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice()->CreateBuffer(&bufferDesc, NULL, &s_pVertexBuffer) );
}
	
void SpriteFont::Flush(void)
{
	if(m_TextQueue.empty() )
		return;

	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();

	s_pMaterial->SetVariable(_T("Texture"), m_pTexture.get() );
	s_pMaterial->SetVariable(_T("TextureDimensions"), m_TextureDimensions);

	//Fill vertex buffer
	TextVertex *pBuffer;
	unsigned int bufferPos = 0;

	s_pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&pBuffer);
	for(auto& txtData : m_TextQueue)
	{
		tt::Vector2 totalAdvance(0);

		for(auto& character : txtData.Text){			
			if(character == _BM'\n'){
				totalAdvance.y += m_FontSize;
				totalAdvance.x = 0;
				continue;
			}

			auto it = m_Characters.find(character);
			
			if(it == m_Characters.end() ){
				s_pVertexBuffer->Unmap();
				throw exception();
			}
			
			if(character == _BM' '){
				totalAdvance.x += it->second.AdvanceX;
				continue;
			}

			pBuffer[bufferPos++] = TextVertex(	 txtData.Position + totalAdvance + it->second.OffsetTexToScreen //Position
												,it->second.TexCoord	//TexCoord
												,it->second.Dimensions  //CharSize
												,txtData.Color			//Color
												,it->second.Channel);   //Channel
			totalAdvance.x += it->second.AdvanceX;
		}
	}
	s_pVertexBuffer->Unmap();

	//DRAW
	D3D10_TECHNIQUE_DESC techDesc;
	s_pMaterial->GetActiveTechnique()->GetDesc( &techDesc );

	for(unsigned int p=0; p < techDesc.Passes; ++p){
		s_pMaterial->GetActiveTechnique()->GetPassByIndex(p)->Apply(0);
		pD3DDevice->Draw(bufferPos, 0); 
	}

	m_TextQueue.clear();
}

void SpriteFont::DrawText(const std::tstring& text, tt::Vector2 position, const tt::Vector4& color)
{
	if(m_NrOfCharsToDraw += text.size() > sc_MaxNrOfChars)
		throw exception();

	MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetSpriteBatch()->AddSpriteFont(this);
	
	m_TextQueue.push_back(TextData(text, position, color));
}
