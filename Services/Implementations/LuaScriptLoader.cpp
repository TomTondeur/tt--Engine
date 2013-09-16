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

#include "../Interfaces/ResourceService.h"
#include "../../Components/ScriptComponent.h"

template<> unique_ptr<ScriptComponent::LuaScript> ResourceService::LoadResource<ScriptComponent::LuaScript>(const std::tstring& filename)
{
	auto pScript = new ScriptComponent::LuaScript(TstringToString(filename));

	if(pScript->m_pLuaState)
		lua_close(pScript->m_pLuaState);

	pScript->m_pLuaState = lua_newstate(&ScriptComponent::LuaAllocate, nullptr);

	if(!pScript->m_pLuaState)
		throw exception("Unable to create new lua state");
	
	luaL_openlibs(pScript->m_pLuaState);
	
	// should return 0 if successful otherwise will return the following:
	//   LUA_ERRFILE  unable to open the file from luaL_loadfile()
	//   LUA_ERRSYNTAX  syntax error in the lua code in the file from lua_load()
	//   LUA_ERRMEM  if there is a memory allocation error from lua_load()
	switch( luaL_loadfile(pScript->m_pLuaState, pScript->m_Filename.c_str() ) ){
	case LUA_ERRFILE:
		throw exception( ("Unable to open file " + pScript->m_Filename + ":\n" + lua_tostring(pScript->m_pLuaState, -1) ).c_str() );
		break;
	case LUA_ERRSYNTAX:
		throw exception( ("Syntax error encountered in " + pScript->m_Filename + ":\n" + lua_tostring(pScript->m_pLuaState, -1) ).c_str() );
		break;
	case LUA_ERRMEM:
		throw exception( ("Memory allocation error encountered in " + pScript->m_Filename + ":\n" + lua_tostring(pScript->m_pLuaState, -1) ).c_str() );
		break;
	case 0:
		break;
	default:
		throw exception( ("Unknown error encountered while loading:\n" + pScript->m_Filename + lua_tostring(pScript->m_pLuaState, -1) ).c_str() );
	}

	return std::unique_ptr<ScriptComponent::LuaScript>(pScript);
}
