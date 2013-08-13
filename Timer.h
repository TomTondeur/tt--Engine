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