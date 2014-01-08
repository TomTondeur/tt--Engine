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
// TTengine.cpp
//

//------------
// Includes
//------------
#include "Helpers/stdafx.h"
#include "TTengine.h"
#include "AbstractGame.h"
#include "Services/ServiceLocator.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/SpriteFont.h"
#include "Scenegraph/GameScene.h"

//------------
// Defines
//------------
using namespace std;

//-----------------------------
// Static Member Initialization
//-----------------------------
TTengine* TTengine::m_pEngineInstance = nullptr;

//---------------------------------
// TTengine Implementation
//---------------------------------

TTengine::TTengine(void):m_pGame(nullptr)
						,m_bProgramTerminated(false)
						,m_FrameCounter(0), m_TimeElapsedSinceLastSecond(0)
{
	
}

TTengine::~TTengine(void)			// Destructor
{

}

TTengine* TTengine::GetInstance(void)
{
	return m_pEngineInstance;
}

void TTengine::Run(void)
{
	//Initialize engine
	m_GameContext.vpInfo.width = 1280;
	m_GameContext.vpInfo.height = 720;
	m_GameContext.pGame = m_pGame;
	m_GameContext.FramesPerSecond = 0;
	Initialize();
	
	//Game loop runs on a separate thread. Main thread is reserved for input monitoring and processing Windows Messages
	DWORD idGameLoopThread;
	auto gameLoopThread = CreateThread(NULL, NULL,(LPTHREAD_START_ROUTINE)GameLoopProc, this, NULL, &idGameLoopThread);
	
	MSG msg={0};
	while(msg.message != WM_QUIT)
	{
		//Message loop
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//Input monitoring
		MyServiceLocator::GetInstance()->GetService<IInputService>()->Update(m_GameContext);
		//Refresh 100x per second (once every 10ms
		Sleep(10);
	}

	//Wait for GameLoop thread to close
	WaitForSingleObject(gameLoopThread, INFINITE);
	CloseHandle(gameLoopThread);
	
}

DWORD TTengine::GameLoopProc(TTengine* pThis)
{
	return pThis->GameLoop();
}

DWORD TTengine::GameLoop(void)
{
	IPhysicsService* pPhysics	= MyServiceLocator::GetInstance()->GetService<IPhysicsService>();
	IGraphicsService* pGraphics = MyServiceLocator::GetInstance()->GetService<IGraphicsService>();

	//Initialize game
	m_pGame->Initialize();
	m_pGame->InitializeGame();
	pPhysics->SetActiveScene(m_pGame->GetActiveScene()->GetPhysicsScene());	
	m_GameContext.GameTimer.Start();

	while(!m_bProgramTerminated){
		//Update
		m_pGame->Update(m_GameContext);
		m_pGame->UpdateGame(m_GameContext);

		pPhysics->SetActiveScene(m_pGame->GetActiveScene()->GetPhysicsScene());
		pPhysics->Simulate(m_GameContext.GameTimer.GetElapsedSeconds()); 
			
		pGraphics->GetGraphicsDevice()->Clear();

		//Draw
		m_pGame->Draw(m_GameContext);
		m_pGame->DrawGame(m_GameContext);

		//Render physics
		pPhysics->FetchResults();
		pPhysics->RenderDebugInfo(m_GameContext);
		
		pGraphics->GetGraphicsDevice()->Present();
		
		//Keep track of time
		m_GameContext.GameTimer.Tick();

		m_TimeElapsedSinceLastSecond += m_GameContext.GameTimer.GetElapsedSeconds();
		if(m_TimeElapsedSinceLastSecond > 1){
			m_TimeElapsedSinceLastSecond -= 1;
			m_GameContext.FramesPerSecond = m_FrameCounter;
			m_FrameCounter = 0;
		}
		m_FrameCounter++;
	}

	m_bProgramTerminated = false;
	
	return 0;
}

void TTengine::Initialize(void)
{
	auto pServiceLoc = MyServiceLocator::GetInstance();
	pServiceLoc->GetService<IGraphicsService>()->InitWindow(m_GameContext.vpInfo.width, m_GameContext.vpInfo.height, this);
	pServiceLoc->GetService<IPhysicsService>()->Initialize();
}

LRESULT CALLBACK TTengine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_CREATE){
		LPCREATESTRUCT csPtr = (LPCREATESTRUCT)lParam;
		SetWindowLongPtr(hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(csPtr->lpCreateParams));
	}else{
		LONG_PTR userDataPtr = GetWindowLongPtr(hWnd,GWLP_USERDATA);
		m_pEngineInstance = reinterpret_cast<TTengine*>(userDataPtr);

		if(m_pEngineInstance != nullptr)
			return m_pEngineInstance->HandleWM(hWnd,message,wParam,lParam);
	}
	return DefWindowProc (hWnd, message, wParam, lParam);
}

LRESULT TTengine::HandleWM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	tstringstream txtBuffer;
	
	switch (message){
		case WM_DESTROY:
			m_bProgramTerminated = true;
			while(m_bProgramTerminated) //GameLoop thread will set this to false when it has performed the last drawcall
				PostQuitMessage(0);
			return 0;
	}
	// Use Default handling for all messages we ignore 
	return DefWindowProc (hWnd, message, wParam, lParam); 
}

void TTengine::SetGame(AbstractGame* pGame)
{
	m_pGame = pGame;
}
