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
// TTscene.h : Sample Scene class
//

#pragma once

#include "Scenegraph/GameScene.h"

class PostProcessingEffect;

class TTscene : public GameScene
{
public:
	//Default constructor & destructor
	TTscene(void);
	virtual ~TTscene(void);

	//Methods
	virtual void Initialize(void) override;
	virtual void Update(const tt::GameContext& context) override;
	virtual void Draw(const tt::GameContext& context) override;

private:
	//Datamembers
	PostProcessingEffect* pPostProEffect;

	//Disabling default copy constructor & assignment operator
	TTscene(const TTscene& src);
	TTscene& operator=(const TTscene& src);
};
