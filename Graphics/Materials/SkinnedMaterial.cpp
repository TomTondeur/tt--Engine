#include "SkinnedMaterial.h"
#include "../../Services/ServiceLocator.h"

SkinnedMaterial::SkinnedMaterial(void):
	Material((L"./Resources/SkinnedEffect.fx")),
	m_vecLightDirection(-.5f, -.5f, .5f)
{}

SkinnedMaterial::~SkinnedMaterial()
{

}

void SkinnedMaterial::SetDiffuse(const std::tstring& diffuseFilename)
{
	m_pTexture = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ID3D10ShaderResourceView>(diffuseFilename);
}

void SkinnedMaterial::SetLightDirection(D3DXVECTOR3 lightDirection)
{
	m_vecLightDirection = lightDirection;
}

void SkinnedMaterial::UpdateEffectVariables(const tt::GameContext& context)
{
	SetVariable(_T("LightDirection"), m_vecLightDirection);
	SetVariable(_T("DiffuseTexture"), m_pTexture.get() );

	if(m_vecBoneTransforms.size() > 0)
		SetVariable(_T("BoneTransforms"), &m_vecBoneTransforms[0], sizeof(D3DXMATRIX) * m_vecBoneTransforms.size() );
}

void SkinnedMaterial::SetBoneTransforms(const vector<D3DXMATRIX>& boneTransforms)
{
	m_vecBoneTransforms = boneTransforms;
}