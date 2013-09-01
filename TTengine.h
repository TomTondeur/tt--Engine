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
// TTengine.h : Bridges Win32 and OOP, contains main game loop
//

#pragma once

//------------
// Includes
//------------
#include "Helpers/Namespace.h"
#include "Timer.h"
#include "Helpers/resrc_ptr.hpp"

//Forward declarations
class AbstractGame;
class SpriteFont;

//--------------------------------
// TTengine Declaration
//--------------------------------
class TTengine
{
	public:
		// Default constructor, destructor
		 TTengine(void);
		virtual ~TTengine(void);
	
		//Overloaded operators
		void Initialize(void);

		// Methods
		static TTengine*	GetInstance(void);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 

		void SetGame(AbstractGame* pGame);
		
		void Run(void);
		
	private:
		// Internal methods
		int InitWindow(HINSTANCE hInstance);

		LRESULT	HandleWM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static DWORD GameLoopProc(TTengine* pThis);
		DWORD GameLoop(void);

		// Datamembers
		static TTengine* m_pEngineInstance;
		AbstractGame* m_pGame;
		bool m_bProgramTerminated;
		tt::GameContext m_GameContext;
		resource_ptr<SpriteFont> m_pDefaultFont;

		unsigned int m_FrameCounter;
		float m_TimeElapsedSinceLastSecond;
		
		//Disabled copy constructor & assignment operator
		TTengine(TTengine& source);// = delete;
		TTengine& operator=(const TTengine& src);// = delete;
};

