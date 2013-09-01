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
// IAudioService.h : Interface for Audio services
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "Service.h"

class IAudioService : public Service
{
public:
	//Default constructor & destructor
	IAudioService(void){}
	virtual ~IAudioService(void){}

	//Methods
	virtual void PlaySound()=0;
	virtual void StopSound()=0;

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	IAudioService(const IAudioService& src);// = delete;
	IAudioService& operator=(const IAudioService& src);// = delete;
};
