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
