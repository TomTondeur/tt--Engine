#include "Material.h"
#include "../Diagnostics/Exceptions.h"
#include "../Services/ServiceLocator.h"
#include "../Services/Interfaces/ResourceService.h"
#include "GraphicsDevice.h"
#include "EffectTechnique.h"
#include "../Components/CameraComponent.h"
#include "../AbstractGame.h"
#include "../Scenegraph/GameScene.h"

Material::Material(std::tstring effectFileName):m_EffectFileName(effectFileName), m_pActiveTechnique(nullptr)
{

}

Material::~Material(void)
{
	for(auto pTech : m_Techniques)
		delete pTech;
}

//Methods

void Material::LoadEffect(void)
{
	//Compile effect
	m_pEffect = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ID3D10Effect>(m_EffectFileName);

	//Get effect info
	D3D10_EFFECT_DESC desc;
	m_pEffect->GetDesc(&desc);

	//Load techniques
	for(unsigned int i=0; i < desc.Techniques; ++i)
		m_Techniques.push_back( new EffectTechnique(m_pEffect->GetTechniqueByIndex(i), i) );
	
	//Load effect variables
	for(unsigned int i=0; i < desc.GlobalVariables; ++i){
		ID3D10EffectVariable* pVariable = m_pEffect->GetVariableByIndex(i);

		D3D10_EFFECT_VARIABLE_DESC varDesc;
		pVariable->GetDesc(&varDesc);
		if(varDesc.Semantic != nullptr)
			m_EffectVariables.insert(make_pair(StringToTstring(varDesc.Semantic), pVariable));
	}

	//Set default active technique
	SetActiveTechnique(0);
}

void Material::UpdateEffectVariables(const tt::GameContext& context, const tt::Matrix4x4& worldMat)
{
	tt::Matrix4x4 viewMat = context.pGame->GetActiveScene()->GetActiveCamera()->GetView();
	tt::Matrix4x4 projMat = context.pGame->GetActiveScene()->GetActiveCamera()->GetProjection();
	
	if(ContainsVariable(_T("World")))
		SetVariable(_T("World"), worldMat);
	
	if(ContainsVariable(_T("View")))
		SetVariable(_T("View"), viewMat);

	if(ContainsVariable(_T("ViewInverse")))
		SetVariable(_T("ViewInverse"), viewMat.Inverse()); //Invert

	if(ContainsVariable(_T("Projection")))
		SetVariable(_T("Projection"), projMat);

	if(ContainsVariable(_T("WorldViewProjection")))
		SetVariable(_T("WorldViewProjection"), worldMat * viewMat * projMat);

	SetVariable(_T("DIRECTION"), tt::Vector3(0,-1,0));
	SetVariable(_T("TEXTURE_DIFFUSE"), MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ID3D10ShaderResourceView>(_T("c:/users/user/desktop/Vampire_Diffuse.dds")).get());
}

InputLayout* Material::GetInputLayout(void) const
{
	return m_pActiveTechnique->GetInputLayout();
}

ID3D10EffectTechnique* Material::GetActiveTechnique(void) const
{
	return m_pActiveTechnique->GetTechnique();
}

void Material::SetActiveTechnique(unsigned int index)
{
	m_pActiveTechnique = GetTechnique(index);		
}

void Material::SetActiveTechnique(std::tstring name)
{
	m_pActiveTechnique = GetTechnique(name);	
}

EffectTechnique* Material::GetTechnique(unsigned int index)
{
	auto it = find_if(m_Techniques.begin(), m_Techniques.end(), [&](EffectTechnique* tech)
						{
							return tech->GetIndex() == index;
						});
	if(it == m_Techniques.end())
		throw EffectTechniqueNotFoundException();
	return *it;
}

EffectTechnique* Material::GetTechnique(std::tstring name)
{
	auto it = find_if(m_Techniques.begin(), m_Techniques.end(), [&](EffectTechnique* tech)
						{
							return tech->GetName() == name;
						});
	if(it == m_Techniques.end())
		throw EffectTechniqueNotFoundException();
	return *it;
}

void Material::SetVariable(std::tstring semantic, const tt::Matrix4x4& value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsMatrix()->SetMatrix( reinterpret_cast<float*>( &value.To_DxMatrix() ) );
}

void Material::SetVariable(std::tstring semantic, const tt::Vector2& value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
		
	it->second->AsVector()->SetFloatVector( reinterpret_cast<float*>( &static_cast<D3DXVECTOR2>(value ) ) );
}

void Material::SetVariable(std::tstring semantic, const tt::Vector3& value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsVector()->SetFloatVector( reinterpret_cast<float*>( &static_cast<D3DXVECTOR3>(value ) ) );
}

void Material::SetVariable(std::tstring semantic, const tt::Vector4& value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsVector()->SetFloatVector( reinterpret_cast<float*>( &static_cast<D3DXVECTOR4>(value ) ) );
}

void Material::SetVariable(std::tstring semantic, int value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsScalar()->SetInt(value);
}

void Material::SetVariable(std::tstring semantic, float value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsScalar()->SetFloat(value);
}

void Material::SetVariable(std::tstring semantic, bool value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsScalar()->SetBool(value);
}

void Material::SetVariable(std::tstring semantic, ID3D10ShaderResourceView* value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsShaderResource()->SetResource(value);
}

bool Material::ContainsVariable(std::tstring semantic)
{
	return m_EffectVariables.find(semantic) != m_EffectVariables.end();
}
