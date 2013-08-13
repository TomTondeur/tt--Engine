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

#include "../../Helpers/stdafx.h"
#include "Service.h"

enum LogLevel
{
	Info,
	Warning,
	Error
};

class DebugService : public Service
{
public:
	//Default constructor & destructor
	DebugService(void);
	virtual ~DebugService(void);

	//Methods
	void Log(const std::tstring& msg, LogLevel level = LogLevel::Info, int line=0, const std::string& file="");
	void LogWin32Error(DWORD errorCode, int line, const std::string& file);
	void LogDirectXError(HRESULT hr, int line, const std::string& file);

private:
	std::tstring GetErrorMessage(DWORD errorCode);

	//Disabling default copy constructor & assignment operator
	DebugService(const DebugService& src);
	DebugService& operator=(const DebugService& src);
};
