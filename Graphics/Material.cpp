// Copyright © 2013 Tom Tondeur
// 
// This file is part of tt::Engine.
// 
// tt::Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// tt::Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with tt::Engine.  If not, see <http://www.gnu.org/licenses/>.

#include "Material.h"
#include "../Diagnostics/Exceptions.h"
#include "../Services/ServiceLocator.h"
#include "../Services/Interfaces/ResourceService.h"
#include "GraphicsDevice.h"
#include "EffectTechnique.h"
#include "../Components/CameraComponent.h"
#include "../AbstractGame.h"
#include "../Scenegraph/GameScene.h"
#include "RenderTarget2D.h"

tt::Matrix4x4 Material::s_DominantDirectionalLightViewProjection = tt::Matrix4x4::Identity;

Material::Material(const std::tstring& effectFileName):m_EffectFileName(effectFileName), m_pActiveTechnique(nullptr)
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

void Material::InitializeEffectVariables(void){}

void Material::Update(const tt::GameContext& context, const tt::Matrix4x4& worldMat)
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
	
	if(ContainsVariable(_T("ShadowMapSRV")))
		SetVariable(_T("ShadowMapSRV"), MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetShadowMapRenderTarget()->GetDepthMap());
	
	if(ContainsVariable(_T("LightViewProjection")))
		SetVariable(_T("LightViewProjection"), worldMat * s_DominantDirectionalLightViewProjection);

	UpdateEffectVariables(context);
}

void Material::GenerateShadows(const tt::GameContext& context, const tt::Matrix4x4& worldMat)
{
	if(ContainsVariable(_T("LightViewProjection")))
		SetVariable(_T("LightViewProjection"), worldMat * s_DominantDirectionalLightViewProjection);

	UpdateEffectVariables(context);
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

void Material::SetActiveTechnique(const std::tstring& name)
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

EffectTechnique* Material::GetTechnique(const std::tstring& name)
{
	auto it = find_if(m_Techniques.begin(), m_Techniques.end(), [&](EffectTechnique* tech)
						{
							return tech->GetName() == name;
						});
	if(it == m_Techniques.end())
		throw EffectTechniqueNotFoundException();
	return *it;
}

void Material::SetVariable(const std::tstring& semantic, const tt::Matrix4x4& value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsMatrix()->SetMatrix( reinterpret_cast<float*>( &static_cast<D3DXMATRIX>(value) ) );
}

void Material::SetVariable(const std::tstring& semantic, const tt::Vector2& value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
		
	it->second->AsVector()->SetFloatVector( reinterpret_cast<float*>( &static_cast<D3DXVECTOR2>(value ) ) );
}

void Material::SetVariable(const std::tstring& semantic, const tt::Vector3& value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsVector()->SetFloatVector( reinterpret_cast<float*>( &static_cast<D3DXVECTOR3>(value ) ) );
}

void Material::SetVariable(const std::tstring& semantic, const tt::Vector4& value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsVector()->SetFloatVector( reinterpret_cast<float*>( &static_cast<D3DXVECTOR4>(value ) ) );
}

void Material::SetVariable(const std::tstring& semantic, int value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsScalar()->SetInt(value);
}

void Material::SetVariable(const std::tstring& semantic, float value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsScalar()->SetFloat(value);
}

void Material::SetVariable(const std::tstring& semantic, bool value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsScalar()->SetBool(value);
}

void Material::SetVariable(const std::tstring& semantic, ID3D10ShaderResourceView* value)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->AsShaderResource()->SetResource(value);
}

void Material::SetVariable(const std::tstring& semantic, void* pRawValue, unsigned int nrOfBytes)
{
	auto it = m_EffectVariables.find(semantic);
	if(it==m_EffectVariables.end())
		throw exception();
	
	it->second->SetRawValue(pRawValue, 0, nrOfBytes);
}

bool Material::ContainsVariable(const std::tstring& semantic)
{
	return m_EffectVariables.find(semantic) != m_EffectVariables.end();
}

void Material::SetDominantDirectionalLightViewProjection(const tt::Matrix4x4& lightMat)
{
	s_DominantDirectionalLightViewProjection = lightMat;
}