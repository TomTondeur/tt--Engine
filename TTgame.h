//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// TTgame.h : Sample Game class
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "Helpers/stdafx.h"
#include "AbstractGame.h"

class ModelComponent;
class GameScene;

//--------------------------------
// TTgame Declaration
//--------------------------------
class TTgame : public AbstractGame
{
public:
	// Default constructor, copy constructor, destructor
	 TTgame(void);
	virtual ~TTgame(void);
	
	//Overloaded operators

	// Methods
	void Initialize(void) override;
	void Update(const tt::GameContext& context) override;
	void Draw(const tt::GameContext& context) override;
	
	// Public members

private:
	// Internal methods

	// Datamembers
	ID3DX10Font* m_pFont;
	
	//Disabled copy constructor
	 TTgame(TTgame& source);
};
