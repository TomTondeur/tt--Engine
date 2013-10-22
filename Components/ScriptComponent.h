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

#include "../Helpers/stdafx.h"
#include "../Scenegraph/ObjectComponent.h"
#include "../Helpers/resrc_ptr.hpp"
#include <LuaLink>

class ScriptComponent : public ObjectComponent
{
	friend class ResourceService;

public:
	template<unsigned int i> struct LuaFunction;
	template<unsigned int i> friend struct LuaFunction;

	//Default constructor & destructor
	ScriptComponent(const std::tstring& filename);
	virtual ~ScriptComponent(void);
	
	// // Opens and loads the file linked to this object
	void Load(void(*initializeEnvironmentFn)(void) = nullptr, bool bOpenLibs = true, bool bResetState = false);
	// // Adds all registered C++ functions and classes to the environment and performs an initial run
	void Initialize(void) override;

	//Calls Update function (if defined in script)
	void Update(const tt::GameContext& context) override;

private:
	//Datamembers
	resource_ptr<LuaLink::LuaScript> m_pScript;
	std::string m_ShortFilename;

	//Disabling default copy constructor & assignment operator
	ScriptComponent(const ScriptComponent& src);
	ScriptComponent& operator=(const ScriptComponent& src);
};
