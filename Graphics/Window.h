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

#include "../Helpers/stdafx.h"

class TTengine;

class Window
{
public:
	//Default constructor & destructor
	Window(unsigned short windowWidth, unsigned short windowHeight);
	virtual ~Window(void);

	//Methods
	void Create(TTengine* pEngine);
	HWND GetHandle(void) const;
	HANDLE GetConsoleWindowHandle(void) const;
	const CONSOLE_SCREEN_BUFFER_INFO& GetDefaultConsoleInfo(void) const;

	void SetName(const std::tstring& name);
	void SetDimensions(UINT width, UINT height);
	void SetPosition(POINT pos);

private:
	//Datamembers
	HWND m_Handle;
	HANDLE m_hConsole;

	POINT m_WindowPos;
	unsigned short m_Width, m_Height;

	DWORD m_Style, m_ExStyle;
	std::tstring m_Name;
	
	CONSOLE_SCREEN_BUFFER_INFO m_DefaultConsoleInfo;

	//Disabling default copy constructor & assignment operator
	Window(const Window& src);
	Window& operator=(const Window& src);
};
