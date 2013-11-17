#include "SkinnedMaterial.h"
#include "../../Services/ServiceLocator.h"
#include "../MeshAnimator.h"

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

void SkinnedMaterial::SetLightDirection(const tt::Vector3& lightDirection)
{
	m_vecLightDirection = lightDirection;
}

void SkinnedMaterial::UpdateEffectVariables(const tt::GameContext& context)
{
	SetVariable(_T("LightDirection"), m_vecLightDirection);
	SetVariable(_T("DiffuseTexture"), m_pTexture.get() );
	/*
	if( !m_vecBoneTransforms.empty() )
		SetVariable(_T("BoneTransforms"), &m_vecBoneTransforms[0], sizeof(D3DXMATRIX) * m_vecBoneTransforms.size() );
	*/
	if( !m_vecDualQuats.empty() )
		SetVariable( _T("DualQuats"), &m_vecDualQuats[0], sizeof(tt::DualQuaternion) * m_vecDualQuats.size() );
}

void SkinnedMaterial::SetBoneTransforms(const vector<D3DXMATRIX>& boneTransforms)
{
	m_vecBoneTransforms = boneTransforms;
}

void SkinnedMaterial::SetDualQuats(const vector<tt::DualQuaternion>& dualQuats)
{
	m_vecDualQuats = dualQuats;
}