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
// IInputService.h : Interface for Input services
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "Service.h"
#include "../../Helpers/stdafx.h"
#include "../../Helpers/Namespace.h"

enum class InputActionId;
enum class KeyState : unsigned char;

class IInputService : public Service
{
public:
	IInputService(){}
	virtual ~IInputService(){}

	virtual void Update(const tt::GameContext& context)=0;
	virtual void AddInputAction(InputActionId action, unsigned char key, KeyState state)=0;
	virtual bool IsActionTriggered(InputActionId action)=0;
	
	virtual tt::Vector2 GetMousePosition(void)=0;
	virtual tt::Vector2 GetMouseMovement(void)=0;

private:
	IInputService(const IInputService& src);// = delete;
	IInputService& operator=(const IInputService& src);// = delete;
};
