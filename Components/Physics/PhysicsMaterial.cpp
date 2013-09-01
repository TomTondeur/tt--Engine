#include "PhysicsMaterial.h"
#include "../../Services/ServiceLocator.h"

PhysicsMaterial::PhysicsMaterial(void) : m_bIsInitialized(false), m_pPhysicsMaterial(nullptr)
{}

PhysicsMaterial::~PhysicsMaterial(void)
{}

//Methods

void PhysicsMaterial::Initialize(void)
{
	m_pPhysicsMaterial = MyServiceLocator::GetInstance()->GetService<IPhysicsService>()->GetActiveScene()->createMaterial(m_MaterialDesc);
	m_bIsInitialized = true;
}

void PhysicsMaterial::SetDynamicFriction(float dynamicFriction)
{
	m_MaterialDesc.dynamicFriction = dynamicFriction;

	if(m_bIsInitialized)
		m_pPhysicsMaterial->setDynamicFriction(dynamicFriction);
}

void PhysicsMaterial::SetStaticFriction(float staticFriction)
{
	m_MaterialDesc.staticFriction = staticFriction;

	if(m_bIsInitialized)
		m_pPhysicsMaterial->setStaticFriction(staticFriction);
}

void PhysicsMaterial::SetRestitution(float restitution)
{
	m_MaterialDesc.restitution = restitution;

	if(m_bIsInitialized)
		m_pPhysicsMaterial->setRestitution(restitution);
}

void PhysicsMaterial::SetDynamicFrictionV(float dynamicFrictionV)
{
	m_MaterialDesc.dynamicFrictionV = dynamicFrictionV;

	if(m_bIsInitialized)
		m_pPhysicsMaterial->setDynamicFrictionV(dynamicFrictionV);
}

void PhysicsMaterial::SetStaticFrictionV(float staticFrictionV)
{
	m_MaterialDesc.staticFrictionV = staticFrictionV;

	if(m_bIsInitialized)
		m_pPhysicsMaterial->setStaticFrictionV(staticFrictionV);
}

void PhysicsMaterial::SetDirectionOfAnisotropy(const tt::Vector3& dirOfAnisotropy)
{
	m_MaterialDesc.dirOfAnisotropy = static_cast<NxVec3>(dirOfAnisotropy);

	if(m_bIsInitialized)
		m_pPhysicsMaterial->setDirOfAnisotropy(dirOfAnisotropy);
}

void PhysicsMaterial::SetFrictionCombineMode(NxCombineMode frictionCombineMode)
{
	m_MaterialDesc.frictionCombineMode = frictionCombineMode;

	if(m_bIsInitialized)
		m_pPhysicsMaterial->setFrictionCombineMode(frictionCombineMode);
}

void PhysicsMaterial::SetRestitutionCombineMode(NxCombineMode restitutionCombineMode)
{
	m_MaterialDesc.restitutionCombineMode = restitutionCombineMode;

	if(m_bIsInitialized)
		m_pPhysicsMaterial->setRestitutionCombineMode(restitutionCombineMode);
}

float PhysicsMaterial::GetDynamicFriction(void) const
{
	return m_MaterialDesc.dynamicFriction;
}

float PhysicsMaterial::GetStaticFriction(void) const
{
	return m_MaterialDesc.staticFriction;
}

float PhysicsMaterial::GetRestitution(void) const
{
	return m_MaterialDesc.restitution;
}

float PhysicsMaterial::GetDynamicFrictionV(void) const
{
	return m_MaterialDesc.dynamicFrictionV;
}

float PhysicsMaterial::GetStaticFrictionV(void) const
{
	return m_MaterialDesc.staticFrictionV;
}

tt::Vector3 PhysicsMaterial::GetDirectionOfAnisotropy(void) const
{
	return tt::Vector3(m_MaterialDesc.dirOfAnisotropy);
}

NxCombineMode PhysicsMaterial::GetFrictionCombineMode(void) const
{
	return m_MaterialDesc.frictionCombineMode;
}

NxCombineMode PhysicsMaterial::GetRestitutionCombineMode(void) const
{
	return m_MaterialDesc.restitutionCombineMode;
}

NxMaterial*	PhysicsMaterial::GetMaterial(void) const
{
	return m_pPhysicsMaterial;
}

bool PhysicsMaterial::IsInitialized(void) const
{
	return m_bIsInitialized;
}
