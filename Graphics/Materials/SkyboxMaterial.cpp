#include "SkyboxMaterial.h"
#include "../../Services/ServiceLocator.h"

SkyboxMaterial::SkyboxMaterial(const std::tstring& cubeMapFilename) : Material(_T("Resources/Effects/SkyBox.fx")), m_CubemapFilename(cubeMapFilename)
{}

SkyboxMaterial::~SkyboxMaterial(void)
{}

//Methods

void SkyboxMaterial::InitializeEffectVariables(void)
{
	m_pTexture = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ID3D10ShaderResourceView>(m_CubemapFilename);
	LoadEffect();
}

void SkyboxMaterial::UpdateEffectVariables(const tt::GameContext& context)
{
	SetVariable(_T("CubeMap"), m_pTexture.get());
}
