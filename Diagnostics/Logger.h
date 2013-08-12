//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// Logger.h : file containing utilities to aid in debugging
// Copyright © 2013 Tom Tondeur
//

#pragma once
#include "../Helpers/stdafx.h"
#include <strsafe.h>

enum LogLevel
{
	Info,
	Warning,
	Error
};

class Logger
{
public:
	static void Log(const std::tstring& msg, LogLevel level = LogLevel::Info, int line=0, const std::string& file="");
	static void LogWin32Error(DWORD errorCode, int line, const std::string& file);
	static void LogDirectXError(HRESULT hr, int line, const std::string& file);


private:
	static std::tstring GetErrorMessage(DWORD errorCode);

	//Pure static class, no need for constructors/destructors/assignment operator
	Logger(void); // = delete;
	~Logger(void){};
	Logger(const Logger& src);// = delete
	Logger& operator=(const Logger& src);// = delete;
};