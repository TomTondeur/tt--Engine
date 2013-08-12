#include "ObjectComponent.h"

#include "SceneObject.h"

ObjectComponent::ObjectComponent():m_bActive(false){}
ObjectComponent::~ObjectComponent(){}
void ObjectComponent::Initialize(){}
void ObjectComponent::Update(const tt::GameContext& context){}
void ObjectComponent::Draw(const tt::GameContext& context){}

void ObjectComponent::SetActive(bool b)
{
	m_bActive = b;
}

bool ObjectComponent::IsActive(void) const
{
	return m_bActive;
}
