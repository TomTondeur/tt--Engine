#pragma once

#include "../Helpers/D3DUtil.h"

enum class InputLayoutSemantic : unsigned char
{
	Position,
	TexCoord,
	Normal,
	Tangent,
	Binormal,
	Color,
	BlendIndices,
	BlendWeights
};

struct InputLayoutElement
{
	InputLayoutSemantic Semantic;
	unsigned int SemanticIndex;
	DXGI_FORMAT Format;
	unsigned int Offset;
	
	bool operator==(const InputLayoutElement& ref);
	bool operator!=(const InputLayoutElement& ref);
};

struct InputLayout
{
	ID3D10InputLayout* pInputLayout;
	std::vector<InputLayoutElement> InputLayoutDesc;
};

class EffectTechnique
{
public:
	//Default constructor & destructor
	EffectTechnique(ID3D10EffectTechnique* pTech, unsigned int index);
	virtual ~EffectTechnique(void);

	//Methods
	unsigned int GetIndex(void) const;
	const std::tstring& GetName(void) const;
	ID3D10EffectTechnique* GetTechnique(void) const;
	InputLayout* GetInputLayout(void) const;

private:
	//Datamembers
	ID3D10EffectTechnique* m_pTechnique;
	//vector<InputLayoutElement> m_InputLayoutDesc;
	unsigned char m_TechniqueIndex;
	std::tstring m_TechniqueName;
	InputLayout* m_pInputLayout;

	static std::vector<InputLayout> m_InputLayouts;
	
	void BuildInputLayout(void);
	InputLayoutElement GetInputLayoutElement(D3D10_SIGNATURE_PARAMETER_DESC& signParDesc);

	//Disabling default copy constructor & assignment operator
	EffectTechnique(const EffectTechnique& src);
	EffectTechnique& operator=(const EffectTechnique& src);
};


//-------------
// Static vector of inputlayouts
// Member index to access correct inputlayout in the vector
// Compare ILElement to check if the new inputlayout is unique
//-------------