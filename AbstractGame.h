//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// AbstractGame.h : Abstract class, game classes inherit from this
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "TTengine.h"

//------------
// Includes
//------------

class GameScene;

//--------------------------------
// AbstractGame Declaration
//--------------------------------
class AbstractGame
{
public:
	// Default constructor, copy constructor, destructor
	 AbstractGame(void);
	virtual ~AbstractGame(void);
	
	//Overloaded operators

	// Methods
	virtual void Initialize(void)=0;
	virtual void Update(const tt::GameContext& context)=0;
	virtual void Draw(const tt::GameContext& context)=0;
	
	void InitializeGame(void);
	void UpdateGame(const tt::GameContext& context);
	void DrawGame(const tt::GameContext& context);

	void SetActiveScene(const std::tstring& name);
	GameScene* GetActiveScene(void) const;
	void AddGameScene(const std::tstring& sceneName, GameScene* pScene);


	// Public members

protected:
	// Internal methods

	// Datamembers


private:
	// Internal methods

	// Datamembers
	std::map<std::tstring, GameScene*> m_GameScenes;
	GameScene* m_pActiveScene;

	//Disabled copy constructor
	 AbstractGame(AbstractGame& source);
};
