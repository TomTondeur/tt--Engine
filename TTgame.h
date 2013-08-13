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
