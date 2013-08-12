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

//Forward declarations
class AbstractGame;

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
		
		//Disabled copy constructor & assignment operator
		TTengine(TTengine& source);// = delete;
		TTengine& operator=(const TTengine& src);// = delete;
};

