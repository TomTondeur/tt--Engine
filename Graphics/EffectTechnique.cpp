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

#include "EffectTechnique.h"
#include "../Services/ServiceLocator.h"
#include "GraphicsDevice.h"
#include "../Diagnostics/Exceptions.h"

std::vector< std::unique_ptr<InputLayout> > EffectTechnique::m_InputLayouts = std::vector< std::unique_ptr<InputLayout> >();

bool InputLayoutElement::operator==(const InputLayoutElement& ref)
{
	return Format == ref.Format
		&& Offset == ref.Offset
		&& Semantic == ref.Semantic
		&& SemanticIndex == ref.SemanticIndex;
}

bool InputLayoutElement::operator!=(const InputLayoutElement& ref)
{
	return !(*this == ref);
}

EffectTechnique::EffectTechnique(ID3D10EffectTechnique* pTech, unsigned int index):m_pTechnique(pTech),m_TechniqueIndex(index)
{
	D3D10_TECHNIQUE_DESC desc;
	m_pTechnique->GetDesc(&desc);
	m_TechniqueName = StringToTstring(desc.Name);

	BuildInputLayout();
}

EffectTechnique::~EffectTechnique(void)
{

}

//Methods

void EffectTechnique::BuildInputLayout(void)
{
	//Shader descs contain info on input layout

	D3D10_PASS_SHADER_DESC passShaderDesc;
	m_pTechnique->GetPassByIndex(0)->GetVertexShaderDesc(&passShaderDesc);
	
	D3D10_EFFECT_SHADER_DESC effectShaderDesc;
	passShaderDesc.pShaderVariable->GetShaderDesc(passShaderDesc.ShaderIndex, &effectShaderDesc);

	D3D10_SIGNATURE_PARAMETER_DESC signParDesc;  //This buffer is used to store info on the current input layout entry
	vector<D3D10_INPUT_ELEMENT_DESC> layoutDesc; //This descriptor is used to create the input layout
	unsigned int inputLayoutSize = 0;

	//Read new inputlayout
	InputLayout* pInputLayout = new InputLayout();
	for(unsigned int i=0; i < effectShaderDesc.NumInputSignatureEntries; ++i)
	{
		passShaderDesc.pShaderVariable->GetInputSignatureElementDesc(passShaderDesc.ShaderIndex,i, &signParDesc);
		InputLayoutElement ilElem = GetInputLayoutElement(signParDesc);
		
		//Add element to descriptor
		pInputLayout->InputLayoutDesc.push_back(ilElem);

		//Create Input Element Desc
		D3D10_INPUT_ELEMENT_DESC inputLayoutElement = {signParDesc.SemanticName, signParDesc.SemanticIndex, ilElem.Format, 0, inputLayoutSize, D3D10_INPUT_PER_VERTEX_DATA, 0};	
		layoutDesc.push_back(inputLayoutElement);
	
		//Increment Position
		inputLayoutSize+=ilElem.Offset;
	}

	//Find inputlayout with same inputlayoutid
	auto it = find_if(m_InputLayouts.begin(), m_InputLayouts.end(), [&](const unique_ptr<InputLayout>& layout)
						{
							unsigned int ilDescSize = layout->InputLayoutDesc.size();
							if(ilDescSize != pInputLayout->InputLayoutDesc.size())
								return false;

							for(unsigned int i=0; i<ilDescSize; ++i)
								if(layout->InputLayoutDesc[i] != pInputLayout->InputLayoutDesc[i])
									return false;

							return true;
						});

	//No need to create a new inputlayout if id's are equal
	if(it != m_InputLayouts.end()){
		m_pInputLayout = it->get();
		return;
	}

	// Get the pass decriptor from the effect technique
    D3D10_PASS_DESC PassDesc;
	m_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	
	auto pDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
	
	// Create the input layout
	HR( pDevice->CreateInputLayout((D3D10_INPUT_ELEMENT_DESC*)&layoutDesc[0], layoutDesc.size(), PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout->pInputLayout) );
	
	//Store new inputlayout
	m_InputLayouts.push_back( unique_ptr<InputLayout>(pInputLayout) );
	m_pInputLayout = m_InputLayouts.back().get();
}

InputLayoutElement EffectTechnique::GetInputLayoutElement(D3D10_SIGNATURE_PARAMETER_DESC& signParDesc)
{
	InputLayoutElement ilElem;

	// MASK LAYOUT
	// 0001 = 1
	// 0011 = 3
	// 0111 = 7
	// 1111 = 15
	// Byte Offset = Active Bits * 4
	// Active Bits = ²log(mask+1) * 4]
	double logMask = log(static_cast<double>(signParDesc.Mask+1));
	ilElem.Offset = static_cast<unsigned int>(logMask*ELOG2_INV + .1) * 4; //0.1 is added to make sure floor rounding is done correctly

	// Determine Semantic Format
	switch(signParDesc.ComponentType)
	{
		case D3D10_REGISTER_COMPONENT_FLOAT32:
			switch(signParDesc.Mask)
			{
				case 1:
					ilElem.Format = DXGI_FORMAT_R32_FLOAT; break;
				case 3:
					ilElem.Format = DXGI_FORMAT_R32G32_FLOAT; break;
				case 7:
					ilElem.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
				default:
					ilElem.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			}
			break;
		case D3D10_REGISTER_COMPONENT_UINT32:
			switch(signParDesc.Mask)
			{
				case 1:
					ilElem.Format = DXGI_FORMAT_R32_UINT; break;
				case 3:
					ilElem.Format = DXGI_FORMAT_R32G32_UINT; break;
				case 7:
					ilElem.Format = DXGI_FORMAT_R32G32B32_UINT; break;
				default:
					ilElem.Format = DXGI_FORMAT_R32G32B32A32_UINT; break;
			}
			break;
		case D3D10_REGISTER_COMPONENT_SINT32:
			switch(signParDesc.Mask)
			{
				case 1:
					ilElem.Format = DXGI_FORMAT_R32_SINT; break;
				case 3:
					ilElem.Format = DXGI_FORMAT_R32G32_SINT; break;
				case 7:
					ilElem.Format = DXGI_FORMAT_R32G32B32_SINT; break;
				default:
					ilElem.Format = DXGI_FORMAT_R32G32B32A32_SINT; break;
			}
			break;
		default: 
			ASSERT(false);
			break;
	}

	//Semantic Type
	if(		strcmp(signParDesc.SemanticName,"POSITION")==0)		ilElem.Semantic = InputLayoutSemantic::Position;
	else if(strcmp(signParDesc.SemanticName,"NORMAL")==0)		ilElem.Semantic = InputLayoutSemantic::Normal;
	else if(strcmp(signParDesc.SemanticName,"COLOR")==0)		ilElem.Semantic = InputLayoutSemantic::Color;
	else if(strcmp(signParDesc.SemanticName,"TEXCOORD")==0)		ilElem.Semantic = InputLayoutSemantic::TexCoord;
	else if(strcmp(signParDesc.SemanticName,"TANGENT")==0)		ilElem.Semantic = InputLayoutSemantic::Tangent;
	else if(strcmp(signParDesc.SemanticName,"BLENDINDICES")==0)	ilElem.Semantic = InputLayoutSemantic::BlendIndices;
	else if(strcmp(signParDesc.SemanticName,"BLENDWEIGHTS")==0)	ilElem.Semantic = InputLayoutSemantic::BlendWeights;
	else 
		MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(tstring(_T("Semantic Type ")) + StringToTstring(signParDesc.SemanticName) + _T(" is not supported for standard meshes!"),LogLevel::Warning, __LINE__, __FILE__);

	//Semantic Index
	ilElem.SemanticIndex = signParDesc.SemanticIndex;

	return ilElem;
}

unsigned int EffectTechnique::GetIndex(void) const
{
	return m_TechniqueIndex;
}

const std::tstring& EffectTechnique::GetName(void) const
{
	return m_TechniqueName;
}

ID3D10EffectTechnique* EffectTechnique::GetTechnique(void) const
{
	return m_pTechnique;
}

InputLayout* EffectTechnique::GetInputLayout(void) const
{
	return m_pInputLayout;
}
