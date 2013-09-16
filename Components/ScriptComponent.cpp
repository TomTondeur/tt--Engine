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

ScriptComponent::LuaScript::LuaScript(std::string filename):m_Filename(filename), m_pLuaState(nullptr), m_pFnInitEnvironment(nullptr){}

ScriptComponent::LuaScript::~LuaScript(void)
{
	if(!m_pLuaState)
		return;

	lua_close(m_pLuaState);
	m_pLuaState = nullptr;
}

ScriptComponent::ScriptComponent(const std::tstring& filename, void (*cb_InitEnvironment)(lua_State* pLuaState) )												
{
	m_pScript = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<LuaScript>(filename);
	m_pScript->m_pFnInitEnvironment = cb_InitEnvironment;
}

ScriptComponent::~ScriptComponent(void){}

//Methods
void ScriptComponent::InitializeEnvironment(void (*pInitializeCallback)(lua_State* pLuaState))
{
	m_pScript->m_pFnInitEnvironment = pInitializeCallback;
}

void ScriptComponent::Initialize(void)
{
	if(m_pScript->m_pFnInitEnvironment)
		m_pScript->m_pFnInitEnvironment(m_pScript->m_pLuaState);
	
	// should return 0 if successful otherwise will return the following:
	//   LUA_ERRRUN unable to run the script.
	// this initial call will run the script loaded and prep the various functions
	// and variables.  It is necessary to call this once in order to run the script
	// so that all the global variables for functions and table names will be set.
	// after this call, we can perform various actions by setting up the Lua stack
	// and then using lua_pcall() again to perform the action.
	if(lua_pcall(m_pScript->m_pLuaState, 0, 0, 0) != 0)
		throw std::exception( lua_tostring(m_pScript->m_pLuaState, -1) );
}

//  The type of the memory-allocation function used by Lua states. The allocator function
//  must provide a functionality similar to realloc, but not exactly the same. Its arguments
//  are:
//     ud, an opaque pointer passed to lua_newstate;
//     ptr, a pointer to the block being allocated/reallocated/freed;
//     osize, the original size of the block;
//     nsize, the new size of the block.
//
//  ptr is NULL if and only if osize is zero. When nsize is zero, the allocator
//  must return NULL; if osize is not zero, it should free the block pointed to by ptr. When nsize
//  is not zero, the allocator returns NULL if and only if it cannot fill the request. When nsize
//  is not zero and osize is zero, the allocator should behave like malloc. When nsize and osize
//  are not zero, the allocator behaves like realloc().
//
//  Lua assumes that the allocator never fails when osize >= nsize.
//
//  See also this documentation http://www.lua.org/manual/5.2/manual.html#lua_Alloc
//
void* ScriptComponent::LuaAllocate(void *ud, void *ptr, size_t osize, size_t nsize)
{
	void *pOut = nullptr;

	if (osize && nsize && ptr) {
		if (osize < nsize)
			pOut = realloc (ptr, nsize);
		else
			pOut = ptr;
	}
	else if (nsize == 0)
		free(ptr);
	else
		pOut = malloc (nsize);

	return pOut;
}