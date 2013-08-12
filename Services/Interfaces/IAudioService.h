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
	IAudioService(void){};
	virtual ~IAudioService(void){};

	//Methods
	virtual void PlaySound()=0;
	virtual void StopSound()=0;

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	IAudioService(const IAudioService& src);// = delete;
	IAudioService& operator=(const IAudioService& src);// = delete;
};
