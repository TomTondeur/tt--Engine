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

#include "../Scenegraph/ObjectComponent.h"
#include "../Helpers/stdafx.h"
#include "../Helpers/TemplateUtil.h"
#include <lua.hpp>
#include "../Helpers/resrc_ptr.hpp"

class ScriptComponent : public ObjectComponent
{
	friend class ResourceService;

	class LuaScript{
	public:
		LuaScript(std::string filename);
		~LuaScript(void);

		std::string m_Filename;
		lua_State* m_pLuaState;
		void (*m_pFnInitEnvironment)(lua_State* pLuaState);
	};

public:
	template<unsigned int i> struct LuaFunction;
	template<unsigned int i> friend struct LuaFunction;

	//Default constructor & destructor
	ScriptComponent(const std::tstring& filename, void (*cb_InitEnvironment)(lua_State* pLuaState) = nullptr);
	virtual ~ScriptComponent(void);

	//Methods
	virtual void InitializeEnvironment(void (*pInitializeCallback)(lua_State* pLuaState));
	virtual void Initialize(void) override;
	virtual void RunScript(bool bReload = false);
	
	template <typename... _ArgTypes>
	void CallLuaFunction(const std::string& functionName, _ArgTypes... arguments)
	{
		lua_getglobal( m_pScript->m_pLuaState, functionName.c_str() );
		if( lua_type(m_pScript->m_pLuaState, lua_gettop(m_pScript->m_pLuaState)) == LUA_TNIL ){
			lua_settop (m_pScript->m_pLuaState, 0);
			throw std::exception( ("Global not found: " + std::string(functionName) ).c_str() );
		}
		pushLuaStack<_ArgTypes...>(m_pScript->m_pLuaState, arguments...);
		
		if (lua_pcall(m_pScript->m_pLuaState, TemplateUtil::GetNrOfArguments<_ArgTypes...>(), 0, 0) != 0) 
				throw std::exception(lua_tostring(m_pScript->m_pLuaState, -1));

	}
	
	template <typename... _ArgTypes>
	void CallLuaMethod(const std::string& tableName, const std::string& functionName, _ArgTypes... arguments)
	{
		lua_getglobal( m_pScript->m_pLuaState, tableName.c_str() );
		if( lua_type(m_pScript->m_pLuaState, lua_gettop(m_pScript->m_pLuaState)) == LUA_TNIL ){
			lua_settop (m_pScript->m_pLuaState, 0);
			throw std::exception( ("Global not found: " + std::string(functionName) ).c_str() );
		}
		lua_getfield(m_pScript->m_pLuaState, -1, functionName.c_str() );
		if(!lua_isfunction(m_pScript->m_pLuaState, -1)){
			lua_settop (m_pScript->m_pLuaState, 0);
			throw std::exception( (std::string(functionName) + " is not a function in " + tableName).c_str() );
		}

		pushLuaStack<_ArgTypes...>(m_pScript->m_pLuaState, arguments...);
		
		if (lua_pcall(m_pScript->m_pLuaState, TemplateUtil::GetNrOfArguments<_ArgTypes...>(), 0, 0) != 0) 
				throw std::exception(lua_tostring(m_pScript->m_pLuaState, -1));

	}
	
	//Templates
	template<typename T> static T	getLuaVariable(lua_State* pLua, int varIdx);		
	template<> static std::string	getLuaVariable(lua_State* pLua, int varIdx) { return std::string(					lua_tostring(pLua, varIdx) );	}
	template<> static int			getLuaVariable(lua_State* pLua, int varIdx) { return static_cast<int>(				lua_tointeger(pLua, varIdx) );	}
	template<> static unsigned int	getLuaVariable(lua_State* pLua, int varIdx) { return static_cast<unsigned int>(		lua_tounsigned(pLua, varIdx) ); }
	template<> static bool			getLuaVariable(lua_State* pLua, int varIdx) { return lua_toboolean(pLua, varIdx) == 0 ? false : true;				}
	template<> static float			getLuaVariable(lua_State* pLua, int varIdx) { return static_cast<float>(			lua_tonumber(pLua, varIdx) );	}
	template<> static double		getLuaVariable(lua_State* pLua, int varIdx) { return static_cast<double>(			lua_tonumber(pLua, varIdx) );	}
		
	template<typename H, typename... T>
	static void pushLuaStack(lua_State* pLua, H data, T... tailData) { pushLuaVariable<H>(pLua, data); pushLuaStack<T...>(pLua, tailData...); }

	template<> static void pushLuaStack(lua_State* pLua, const char* data)		{ lua_pushstring( pLua,		data);			}
	template<> static void pushLuaStack(lua_State* pLua, std::string data)		{ lua_pushstring( pLua,		data.c_str() );	}
	template<> static void pushLuaStack(lua_State* pLua, const wchar_t* data)	{ lua_pushlstring( pLua,	reinterpret_cast<const char*>(data), (wcslen(data)+1)*sizeof(wchar_t) );	}
	template<> static void pushLuaStack(lua_State* pLua, std::wstring data)		{ lua_pushlstring( pLua,	reinterpret_cast<const char*>(data.c_str() ), (data.size()+1)*sizeof(wchar_t) );	}
	template<> static void pushLuaStack(lua_State* pLua, int data)				{ lua_pushinteger(pLua,		static_cast<lua_Integer>(data) );	}
	template<> static void pushLuaStack(lua_State* pLua, unsigned int data)		{ lua_pushunsigned(pLua,	static_cast<lua_Unsigned>(data) );	}
	template<> static void pushLuaStack(lua_State* pLua, bool data)				{ lua_pushboolean(pLua,		static_cast<int>(data) );			}
	template<> static void pushLuaStack(lua_State* pLua, double data)			{ lua_pushnumber(pLua,		static_cast<lua_Number>(data) );	}
	template<> static void pushLuaStack(lua_State* pLua, float  data)			{ lua_pushnumber(pLua,		static_cast<lua_Number>(data) );	}
	
private:
	//Datamembers
	resource_ptr<LuaScript> m_pScript;

	//Private methods
	static void* LuaAllocate(void *ud, void *ptr, size_t osize, size_t nsize);

	//Disabling default copy constructor & assignment operator
	ScriptComponent(const ScriptComponent& src);
	ScriptComponent& operator=(const ScriptComponent& src);
};

template<unsigned int i>
struct LuaFunction{
private:
	template<typename _RetType, typename... _ArgTypes> struct LuaFunctionGenerator;
	
public:
	//no ret, 0 arg
	static void Register(void(*pFunc)(), const std::string& name, lua_State* luaState){
		LuaFunctionGen::pFunc = pFunc;
		lua_pushcclosure (luaState, LuaFunctionGen::execute, 0);
		lua_setglobal (luaState, name.c_str() );
	}
	//	 ret, 0 arg
	template<typename _RetType>
	static void Register(_RetType(*pFunc)(), const std::string& name, lua_State* luaState){
		LuaFunctionGenerator<_RetType, void>::pFunc = pFunc;
		lua_pushcclosure (luaState, LuaFunctionGenerator<_RetType, void>::execute, 0);
		lua_setglobal (luaState, name.c_str() );
	}
	// 1 arg
	template<typename _RetType, typename _Arg1Type>
	static void Register(_RetType(*pFunc)(_Arg1Type), const std::string& name, lua_State* luaState){
		LuaFunctionGenerator<_RetType, _Arg1Type>::pFunc = pFunc;
		lua_pushcclosure (luaState, LuaFunctionGenerator<_RetType, _Arg1Type>::execute, 0);
		lua_setglobal (luaState, name.c_str() );
	}
	// 2 arg
	template<typename _RetType, typename _Arg1Type, typename _Arg2Type>
	static void Register(_RetType(*pFunc)(_Arg1Type, _Arg2Type), const std::string& name, lua_State* luaState){
		LuaFunctionGenerator<_RetType, _Arg1Type, _Arg2Type>::pFunc = pFunc;
		lua_pushcclosure (luaState, LuaFunctionGenerator<_RetType, _Arg1Type, _Arg2Type>::execute, 0);
		lua_setglobal (luaState, name.c_str() );
	}	

	//IMPLEMENTATION
private:
	//no ret, 0 arg
	struct LuaFunctionGen{
		static int execute(lua_State* pLuaState){
			int argc = lua_gettop(pLuaState);

			if(argc == 0){
				pFunc();
			}
			return 0;
		}
				
		static void(*pFunc)(void);
	};	
	//no ret, 1 arg
	template<typename _Arg1Type>
	struct LuaFunctionGenerator<void, _Arg1Type>{
		static int execute(lua_State* pLuaState){
			int argc = lua_gettop(pLuaState);

			if(argc == 1){
				_Arg1Type arg1 = ScriptComponent::getLuaVariable<_Arg1Type>(pLuaState, 1);	

				pFunc(arg1);
			}
			return 1;
		}
				
		static void(*pFunc)(_Arg1Type);
	};
	//no ret, 2 arg
	template<typename _Arg1Type, typename _Arg2Type>
	struct LuaFunctionGenerator<void, _Arg1Type, _Arg2Type>{
		static int execute(lua_State* pLuaState){
			int argc = lua_gettop(pLuaState);

			if(argc == 2){
				_Arg1Type arg1 = ScriptComponent::getLuaVariable<_Arg1Type>(pLuaState, 1);	
				_Arg2Type arg2 = ScriptComponent::getLuaVariable<_Arg2Type>(pLuaState, 2);

				pFunc(arg1, arg2);
			}
			return 1;
		}
				
		static void(*pFunc)(_Arg1Type, _Arg2Type);
	};
	//	 ret, 0 arg
	template<typename _RetType>
	struct LuaFunctionGenerator<_RetType, void>{
		static int execute(lua_State* pLuaState){
			int argc = lua_gettop(pLuaState);

			if(argc == 0){
				ScriptComponent::pushLuaStack<_RetType>( pLuaState, pFunc() );
				return 1;
			}
			return 0;
		}
				
		static _RetType(*pFunc)(void);
	};
	//	 ret, 1 arg
	template<typename _RetType, typename _Arg1Type>
	struct LuaFunctionGenerator<_RetType, _Arg1Type>{
		static int execute(lua_State* pLuaState){
			int argc = lua_gettop(pLuaState);

			if(argc == 1){
				_Arg1Type arg1 = ScriptComponent::getLuaVariable<_Arg1Type>(pLuaState, 1);	
				ScriptComponent::pushLuaStack<_RetType>( pLuaState, pFunc(arg1) );
				return 1;
			}
			return 0;
		}
				
		static _RetType(*pFunc)(_Arg1Type);
	};
	//	 ret, 2 arg
	template<typename _RetType, typename _Arg1Type, typename _Arg2Type>
	struct LuaFunctionGenerator<_RetType, _Arg1Type, _Arg2Type>{
		static int execute(lua_State* pLuaState){
			int argc = lua_gettop(pLuaState);

			if(argc == 2){
				_Arg1Type arg1 = ScriptComponent::getLuaVariable<_Arg1Type>(pLuaState, 1);
				_Arg2Type arg2 = ScriptComponent::getLuaVariable<_Arg2Type>(pLuaState, 2);	
				ScriptComponent::pushLuaStack<_RetType>( pLuaState, pFunc(arg1, arg2) );
				return 2;
			}
			return 0;
		}
				
		static _RetType(*pFunc)(_Arg1Type, _Arg2Type);
	};
};

template<unsigned int i> //no ret, 0 arg
void(*LuaFunction<i>::LuaFunctionGen::pFunc)() = nullptr;

template<unsigned int i> //no ret, 1 arg
template<typename _Arg1Type>
void(*LuaFunction<i>::LuaFunctionGenerator<void, _Arg1Type>::pFunc)(_Arg1Type) = nullptr;

template<unsigned int i> //no ret, 2 arg
template<typename _Arg1Type, typename _Arg2Type>
void(*LuaFunction<i>::LuaFunctionGenerator<void, _Arg1Type, _Arg2Type>::pFunc)(_Arg1Type, _Arg2Type) = nullptr;

template<unsigned int i> //   ret, 0 arg
template<typename _RetType>
_RetType(*LuaFunction<i>::LuaFunctionGenerator<_RetType, void>::pFunc)() = nullptr;

template<unsigned int i> //   ret, 1 arg
template<typename _RetType, typename _Arg1Type>
_RetType(*LuaFunction<i>::LuaFunctionGenerator<_RetType, _Arg1Type>::pFunc)(_Arg1Type) = nullptr;

template<unsigned int i> //   ret, 2 arg
template<typename _RetType, typename _Arg1Type, typename _Arg2Type>
_RetType(*LuaFunction<i>::LuaFunctionGenerator<_RetType, _Arg1Type, _Arg2Type>::pFunc)(_Arg1Type, _Arg2Type) = nullptr;
