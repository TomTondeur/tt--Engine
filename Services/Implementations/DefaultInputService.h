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

#pragma once

#include "../Interfaces/IInputService.h"
#include "../../Timer.h"

struct InputAction{
	InputAction(unsigned char _key, KeyState _targetState);

	unsigned char key;
	KeyState targetState;
	KeyState currentState;
	unsigned short milliSecondsSincePress;
};

class DefaultInputService : public IInputService
{
public:
	DefaultInputService();
	virtual ~DefaultInputService();

	virtual void Update(const tt::GameContext& context) override;
	virtual void AddInputAction(InputActionId action, unsigned char key, KeyState state) override;
	virtual bool IsActionTriggered(InputActionId action) override;
	virtual tt::Vector2 GetMousePosition(void) override;
	virtual tt::Vector2 GetMouseMovement(void) override;

private:
	static const unsigned short sc_MilliSecondsUntilHeld = 1000;
	tt::Timer m_UpdateTimer;
	tt::Vector2 m_OldMousePosition, m_MousePosition, m_MouseMovement;

	std::map<InputActionId, InputAction> m_ActionsToMonitor;

	DefaultInputService(const DefaultInputService& src);// = delete;
	DefaultInputService& operator=(const DefaultInputService& src);// = delete;
};
