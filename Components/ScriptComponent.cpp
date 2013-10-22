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

#include "ScriptComponent.h"
#include "../Services/ServiceLocator.h"

using namespace LuaLink;

ScriptComponent::ScriptComponent(const std::tstring& filename)												
{
	m_pScript = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<LuaScript>(filename);
	auto slashIdx = filename.find_last_of(_T('/'));
	m_ShortFilename = TstringToString(filename.substr(slashIdx+1, filename.find_last_of(_T('.')) - slashIdx - 1));
}

ScriptComponent::~ScriptComponent(void){}

//Methods
void ScriptComponent::Load(void(*initializeEnvironmentFn)(void), bool bOpenLibs, bool bResetState)
{
	m_pScript->Load(initializeEnvironmentFn, bOpenLibs, bResetState);
}

void ScriptComponent::Initialize(void)
{
	m_pScript->Initialize();
	try{
		LuaScript::Call<void>::LuaStaticMethod(m_ShortFilename,"Initialize");
	}catch(LuaCallException&)
	{
		std::tstringstream ss; 
		ss << _T("Script ") << StringToTstring(m_ShortFilename) << _T(" does not contain a defintion for ") << StringToTstring(m_ShortFilename) << _T(":Initialize()");
		MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(ss.str(), LogLevel::Warning);
	}
}

void ScriptComponent::Update(const tt::GameContext& context)
{
	try{
		LuaScript::Call<void>::LuaStaticMethod(m_ShortFilename,"Update",context.GameTimer.GetElapsedSeconds());
	}catch(LuaCallException&)
	{
		std::tstringstream ss; 
		ss << _T("Script ") << StringToTstring(m_ShortFilename) << _T(" does not contain a defintion for ") << StringToTstring(m_ShortFilename) << _T(":Update(float dTime)");
		MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(ss.str(), LogLevel::Warning);
	}
}
