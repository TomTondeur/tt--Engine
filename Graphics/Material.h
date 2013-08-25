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
	explicit Material(const std::tstring& effectFileName);
	virtual ~Material(void);

	//Methods
	void LoadEffect(void);
	void Update(const tt::GameContext& context, const tt::Matrix4x4& worldMat);
	virtual void UpdateEffectVariables(const tt::GameContext& context)=0;
	virtual void InitializeEffectVariables(void);

	InputLayout* GetInputLayout(void) const;
	ID3D10EffectTechnique* GetActiveTechnique(void) const;

	void SetActiveTechnique(unsigned int index);
	void SetActiveTechnique(const std::tstring& name);
	
	void SetVariable(const std::tstring& semantic, const tt::Matrix4x4& value);
	void SetVariable(const std::tstring& semantic, const tt::Vector2& value);
	void SetVariable(const std::tstring& semantic, const tt::Vector3& value);
	void SetVariable(const std::tstring& semantic, const tt::Vector4& value);
	void SetVariable(const std::tstring& semantic, int value);
	void SetVariable(const std::tstring& semantic, float value);
	void SetVariable(const std::tstring& semantic, bool value);
	void SetVariable(const std::tstring& semantic, ID3D10ShaderResourceView* value);
	void SetVariable(const std::tstring& semantic, void* pRawData, unsigned int nrOfBytes);

	bool ContainsVariable(const std::tstring& semantic);

private:
	//Datamembers
	std::tstring m_EffectFileName;

	resource_ptr<ID3D10Effect> m_pEffect;
	std::map<std::tstring, ID3D10EffectVariable*> m_EffectVariables;
	
	vector<EffectTechnique*> m_Techniques;
	EffectTechnique* m_pActiveTechnique;

	
	EffectTechnique* GetTechnique(unsigned int index);
	EffectTechnique* GetTechnique(const std::tstring& name);
	
	//Disabling default copy constructor & assignment operator
	Material(const Material& src);
	Material& operator=(const Material& src);
};
