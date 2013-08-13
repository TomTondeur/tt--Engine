//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// Timer.h : file containing class that implements high resolution timer
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "Helpers/stdafx.h"

namespace tt
{
	class Timer final
	{
	public:
		Timer(void);
		~Timer(void);

		void Start(void);
		void Tick(void);

		float GetElapsedSeconds(void) const;
		float GetTotalSeconds(void) const;

	private:
		LARGE_INTEGER m_CountsAtStart, m_CountsLatestTick, m_CountsPreviousTick;

		static LARGE_INTEGER m_Frequency;

		Timer(const Timer& src);// = delete;
		Timer& operator=(const Timer& src);// = delete;
	};
}