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