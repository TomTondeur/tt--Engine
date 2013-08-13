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
// AbstractGame.cpp : Copyright © 2013 Tom Tondeur
//

//------------
// Includes
//------------
#include "Helpers/stdafx.h"
#include "AbstractGame.h"
#include "Scenegraph/GameScene.h"

//------------
// Defines
//------------


using namespace std;

//---------------------------------
// AbstractGame Implementation
//---------------------------------

AbstractGame::AbstractGame(void)			// Default constructor
{

}

AbstractGame::~AbstractGame(void)			// Destructor
{
	for(auto pScene : m_GameScenes)
		delete pScene.second;
}

void AbstractGame::InitializeGame(void)
{
	m_pActiveScene->Initialize();
	m_pActiveScene->InitializeScene();
}

void AbstractGame::UpdateGame(const tt::GameContext& context)
{
	m_pActiveScene->Update(context);
	m_pActiveScene->UpdateScene(context);
}

void AbstractGame::DrawGame(const tt::GameContext& context)
{
	m_pActiveScene->Draw(context);
	m_pActiveScene->DrawScene(context);
}

void AbstractGame::SetActiveScene(const std::tstring& name)
{
	auto it = m_GameScenes.find(name);
	
	if(it == m_GameScenes.end())
		throw exception();

	m_pActiveScene = it->second;
}

void AbstractGame::AddGameScene(const std::tstring& sceneName, GameScene* pScene)
{
	m_GameScenes.insert( make_pair(sceneName, pScene) );
}

GameScene* AbstractGame::GetActiveScene(void) const
{
	return m_pActiveScene;
}