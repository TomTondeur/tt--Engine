#include "Skybox.h"

#include "../Graphics/Materials/SkyboxMaterial.h"
#include "../Services/ServiceLocator.h"
using namespace std;
Skybox::Skybox(void)
{}

Skybox::~Skybox(void)
{}

//Methods


void Skybox::Initialize(void)
{
	m_pMaterial = unique_ptr<Material>(new SkyboxMaterial(_T("Resources/Textures/Skybox.dds")));
	m_pMaterial->InitializeEffectVariables();
	
	auto pTransform = new TransformComponent();
	auto pModelComp = new ModelComponent(_T("Resources/Models/Box.bin"),pTransform);
	pModelComp->SetMaterial(m_pMaterial);

	SetComponent<TransformComponent>(pTransform);
	SetComponent<ModelComponent>(pModelComp);
}

void Skybox::Draw(const tt::GameContext& context)
{
	GetComponent<ModelComponent>()->Draw(context);
}