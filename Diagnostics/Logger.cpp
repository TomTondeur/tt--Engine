//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// Logger.cpp - Copyright © 2013 Tom Tondeur
//

#include "Logger.h"
#include "../TTengine.h"
#include "../Services/ServiceLocator.h"
#include "../Graphics/GraphicsDevice.h"
#include "../Graphics/Window.h"

using namespace std;

//-------
// Logger
//-------

void Logger::Log(const std::tstring& msg, LogLevel level, int line, const string& file)
{
	Window* pWindow = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetWindow();
	HANDLE consoleHandle = pWindow->GetConsoleWindowHandle();
	HWND hWnd = pWindow->GetHandle();

	switch(level)
	{
	case Info:
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
		tcout << _T("[INFO]    ") << msg << endl;
		break;
	
	case Warning:
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);
		tcout << _T("[WARNING] ") << msg << endl;
		break;
	
	case Error:
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY|FOREGROUND_RED);

		if(line!=0 && file!=""){
			tstring msgEx = msg + _T("@ ") + to_tstring(line) + _T(" : ") + tstring(file.begin(),file.end());
			tcout << _T("[ERROR]   ") << msgEx << endl;
			OutputDebugString(msgEx.c_str());
			MessageBox(hWnd, msgEx.c_str(), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		}
		else{
			tcout << _T("[ERROR]   ") << msg << endl;
			OutputDebugString(msg.c_str());
			MessageBox(hWnd, msg.c_str(), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		}

		DEBUG_BREAK
		break;
	}

	// Revert to original console attributes
	SetConsoleTextAttribute( consoleHandle, pWindow->GetDefaultConsoleInfo().wAttributes );
}

void Logger::LogWin32Error(DWORD errorCode, int line, const string& file)
{
	tstring msg = GetErrorMessage(errorCode);
	Log(msg, LogLevel::Error, line, file);
}

void Logger::LogDirectXError(HRESULT hr, int line, const string& file)
{
	tstring msg = tstring(_T("Error Code: ")) + DXGetErrorString(hr) + _T("\nError description: ") + DXGetErrorDescription(hr) + _T("\n");
	Log(msg,LogLevel::Error,line,file);
}

std::tstring Logger::GetErrorMessage(DWORD errorCode)
{
	//Obtain error message from error code
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	//Convert to tstring representation
	unsigned int length = lstrlen((LPCTSTR)lpMsgBuf);
	tstring result=_T("");

	for(UINT i=0; i < length; ++i)
		result += *((LPCTSTR)lpMsgBuf + i);

	//Free message buffer
	LocalFree(lpMsgBuf);

	return result;
}
