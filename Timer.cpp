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
	m_CountsLatestTick = m_CountsAtStart;
	m_CountsPreviousTick = m_CountsAtStart;
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

