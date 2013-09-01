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

//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
// Copyright © 2013 Tom Tondeur
//

#pragma once

//#include "targetver.h"
#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_
#endif

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers, disables fileopendialog
//PhysX:you must suppress its definition of the macros min and max, as these are common C++ method names in the SDK
#define NOMINMAX
// Windows Header Files:
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

//#undef min;
//#undef max;
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <crtdbg.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <map>

#include <NxPhysics.h>

//---------------
//tstrings 4 laif
//---------------
#ifdef _UNICODE 
	namespace std
	{
		typedef wstring tstring;
		typedef wstringstream tstringstream;
		typedef wofstream tofstream;
		typedef wifstream tifstream;
		typedef wfstream tfstream;
	}
	#define tcin wcin
	#define tcout wcout 
	#define __TR LR
	#define to_tstring to_wstring

#define WstringToTstring(x) (x)
	inline std::tstring StringToTstring(const std::string& str)
	{
		return std::tstring(str.begin(),str.end());
	}
#else
	namespace std
	{
		typedef string tstring;
		typedef stringstream tstringstream;
		typedef ofstream tofstream;
		typedef ifstream tifstream;
		typedef fstream tfstream;
	}
	#define tcin cin
	#define tcout cout 
	#define __TR   
	#define to_tstring to_string

	#define StringToTstring(x) (x)
	inline std::tstring WstringToTstring(const std::wstring& str)
	{
		return std::tstring(str.begin(),str.end());
	}

#endif 
	
//-------------
// Assert Macro
//-------------
#ifndef NDEBUG
	#define DEBUG_BREAK __asm {int 3};

	#define ASSERT \
	if ( false ) {} \
	else \
	struct LocalAssert { \
		int mLine; \
		LocalAssert(int line=__LINE__) : mLine(line) {} \
		LocalAssert(bool isOK, const TCHAR* message=_T("")) { \
			if ( !isOK ) { \
				std::tstringstream buffer; \
				buffer << _T("ERROR!! Assert failed on line ") << LocalAssert().mLine << _T(" in file '") << __FILE__ << _T("'\nMessage: \"") << message << _T("\"\n"); \
				OutputDebugString(buffer.str().c_str()); \
				MessageBox(NULL,buffer.str().c_str(),NULL,MB_OK); \
				DEBUG_BREAK \
			} \
		} \
	} myAsserter = LocalAssert
#else
	#define DEBUG_BREAK

	#define ASSERT \
	if ( true ) {} else \
	struct NoAssert { \
		NoAssert(bool isOK, const TCHAR* message=_T("")) {} \
	} myAsserter = NoAssert
#endif
