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

//Include this file when implementing IInputService

enum class KeyState : unsigned char{
	Idle	= 1 << 0,
	Pressed	= 1 << 1,
	Held	= 1 << 2,
	Released= 1 << 3
};

inline KeyState operator|(KeyState a, KeyState b)
{return static_cast<KeyState>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));}

inline bool operator&(KeyState a, KeyState b)
{return (static_cast<unsigned char>(a) & static_cast<unsigned char>(b)) != 0;}

enum class InputActionId
{
	Jump,
	CameraMoveForward,
	CameraMoveBack,
	CameraMoveLeft,
	CameraMoveRight,
	CameraMoveUp,
	CameraMoveDown,
	CameraUnlockRotation,
	ToggleVSync
};
