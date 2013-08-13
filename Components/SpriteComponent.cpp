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

#include "SpriteComponent.h"
#include "../Services/ServiceLocator.h"
#include "../Graphics/SpriteBatch.h"
#include "TransformComponent.h"

Sprite::Sprite(const tt::Matrix4x4& transform) : pTexture(nullptr)
												,Transform(transform)
												,Color(1)
{

}

SpriteComponent::SpriteComponent(std::tstring textureFilename, const TransformComponent* pTransform):m_TextureFilename(textureFilename)
																									,m_Sprite(pTransform->GetWorldMatrix())
{ }

SpriteComponent::~SpriteComponent(void)
{

}

//Methods

void SpriteComponent::Initialize(void)
{
	m_Sprite.pTexture = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ID3D10ShaderResourceView>(m_TextureFilename);
}

void SpriteComponent::Draw(const tt::GameContext& context)
{
	MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetSpriteBatch()->Draw(m_Sprite);
}