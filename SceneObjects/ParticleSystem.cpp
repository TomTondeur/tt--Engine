#include "ParticleSystem.h"
#include "../Graphics/Materials/ParticlesMaterial.h"
#include "../Services/ServiceLocator.h"

ParticleSystem::ParticleSystem(void)
{

}

ParticleSystem::~ParticleSystem(void)
{

}

//Methods

void ParticleSystem::Initialize(void)
{
	auto pTransform = new TransformComponent();
	SetComponent<TransformComponent>(pTransform);

	auto pMat = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ParticlesMaterial>(_T("Resources/Particles/Particles.fx") );

	SetComponent<ParticleEmitterComponent>(new ParticleEmitterComponent(pMat, pTransform) );
}
