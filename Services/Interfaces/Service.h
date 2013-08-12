//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// Service.h : Base class for use with the ServiceLocator
// Copyright © 2013 Tom Tondeur
//

#pragma once

class Service
{
public:
	//Default constructor & destructor
	Service(void){};
	virtual ~Service(void){};

	//Methods

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	Service(const Service& src);// = delete;
	Service& operator=(const Service& src);// = delete;
};
