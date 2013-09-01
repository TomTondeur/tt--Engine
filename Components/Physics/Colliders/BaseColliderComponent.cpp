#include "BaseColliderComponent.h"
#include "../PhysicsMaterial.h"

BaseColliderComponent::BaseColliderComponent(RigidBodyComponent* pRigidBody) : m_pPhysicsMaterial(nullptr), m_bIsTrigger(false), m_pRigidBody(pRigidBody)
{

}

BaseColliderComponent::~BaseColliderComponent(void)
{

}

//Methods

void BaseColliderComponent::SetPhysicsMaterial(PhysicsMaterial* pPhysicsMaterial)
{
	m_pPhysicsMaterial = pPhysicsMaterial;
	Initialize();
}

PhysicsMaterial* BaseColliderComponent::GetPhysicsMaterial(void) const
{
	return m_pPhysicsMaterial;
}

bool BaseColliderComponent::IsTrigger(void) const
{
	return m_bIsTrigger;
}

void BaseColliderComponent::SetAsTrigger(bool value)
{
	m_bIsTrigger = value;
}

unsigned int BaseColliderComponent::GetMaterialIndex(void)
{
	if (!m_pPhysicsMaterial)
		return 0;

	if(!m_pPhysicsMaterial->IsInitialized())
		m_pPhysicsMaterial->Initialize();
	
	return m_pPhysicsMaterial->GetMaterial()->getMaterialIndex();
}
