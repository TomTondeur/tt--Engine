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
// Entrypoint.cpp - Copyright © 2013 Tom Tondeur
//

//---------
// Includes
//---------
#include "Helpers/stdafx.h"
#include "TTengine.h"
#include "TTgame.h"

#pragma comment(lib, "PhysXLoader.lib")
#pragma comment(lib, "NxCharacter.lib")

//------------
// Defines
//------------

using namespace std;

//-------------------
// Windows entrypoint
//-------------------

int _tmain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR nCmdLine, int nCmdShow)
{
	SetConsoleTitle(_T("TTgame debug console"));

	return _tWinMain(hInstance, hPrevInstance, nCmdLine, SW_SHOW);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR nCmdLine, int nCmdShow)
{
	//notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL,0);

	// Enable run-time memory leak check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		//_CrtSetBreakAlloc(827);
	#endif
	
	TTengine* pEngine = new TTengine();
	ASSERT(pEngine != nullptr, _T("Failed to create tt::Engine instance"));

	auto pGame = new TTgame();
	pEngine->SetGame(pGame);

	pEngine->Run();

	delete pGame;
	delete pEngine;
	return 0;
}