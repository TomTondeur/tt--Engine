//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// Timer.cpp - Copyright © 2013 Tom Tondeur
//

#include "Timer.h"
#include "Services/ServiceLocator.h"
#include "Helpers/D3DUtil.h"

using namespace tt;

LARGE_INTEGER Timer::m_Frequency = {0};

Timer::Timer(void)
{
	ZeroInit(m_CountsAtStart);
	ZeroInit(m_CountsLatestTick);
	ZeroInit(m_CountsPreviousTick);

	if(m_Frequency.QuadPart == 0 && QueryPerformanceFrequency(&m_Frequency) == 0)
			MyServiceLocator::GetInstance()->GetService<DebugService>()->LogWin32Error(GetLastError(),__LINE__, __FILE__);
}

Timer::~Timer(void)
{

}

void Timer::Start(void)
{
	if(QueryPerformanceCounter(&m_CountsAtStart) == 0)
		MyServiceLocator::GetInstance()->GetService<DebugService>()->LogWin32Error(GetLastError(),__LINE__, __FILE__);
}

void Timer::Tick(void)
{
	m_CountsPreviousTick = m_CountsLatestTick;
	
	if(QueryPerformanceCounter(&m_CountsLatestTick) == 0)
		MyServiceLocator::GetInstance()->GetService<DebugService>()->LogWin32Error(GetLastError(),__LINE__, __FILE__);
}

float Timer::GetElapsedSeconds(void) const
{
	return static_cast<float>((m_CountsLatestTick.QuadPart - m_CountsPreviousTick.QuadPart) / (double)m_Frequency.QuadPart);
}

float Timer::GetTotalSeconds(void) const
{
	return static_cast<float>((m_CountsLatestTick.QuadPart - m_CountsAtStart.QuadPart) / (double)m_Frequency.QuadPart);
}

