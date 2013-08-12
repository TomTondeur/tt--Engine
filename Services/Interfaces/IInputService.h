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
