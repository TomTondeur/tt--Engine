#pragma once

#include "../Helpers/D3DUtil.h"
#include "../Helpers/stdafx.h"
#include "../Helpers/resrc_ptr.hpp"
#include "../Helpers/Namespace.h"

struct GameContext;

class EffectTechnique;
struct InputLayout;

class Material
{
public:
	//Default constructor & destructor
	explicit Material(std::tstring effectFileName);
	virtual ~Material(void);

	//Methods
	void LoadEffect(void);
	void Update(const tt::GameContext& context, const tt::Matrix4x4& worldMat);
	virtual void UpdateEffectVariables(const tt::GameContext& context)=0;

	InputLayout* GetInputLayout(void) const;
	ID3D10EffectTechnique* GetActiveTechnique(void) const;

	void SetActiveTechnique(unsigned int index);
	void SetActiveTechnique(std::tstring name);
	
	void SetVariable(std::tstring semantic, const tt::Matrix4x4& value);
	void SetVariable(std::tstring semantic, const tt::Vector2& value);
	void SetVariable(std::tstring semantic, const tt::Vector3& value);
	void SetVariable(std::tstring semantic, const tt::Vector4& value);
	void SetVariable(std::tstring semantic, int value);
	void SetVariable(std::tstring semantic, float value);
	void SetVariable(std::tstring semantic, bool value);
	void SetVariable(std::tstring semantic, ID3D10ShaderResourceView* value);

	bool ContainsVariable(std::tstring semantic);

private:
	//Datamembers
	std::tstring m_EffectFileName;

	resource_ptr<ID3D10Effect> m_pEffect;
	std::map<std::tstring, ID3D10EffectVariable*> m_EffectVariables;
	
	vector<EffectTechnique*> m_Techniques;
	EffectTechnique* m_pActiveTechnique;

	
	EffectTechnique* GetTechnique(unsigned int index);
	EffectTechnique* GetTechnique(std::tstring name);
	
	//Disabling default copy constructor & assignment operator
	Material(const Material& src);
	Material& operator=(const Material& src);
};
