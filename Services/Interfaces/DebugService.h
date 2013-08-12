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
