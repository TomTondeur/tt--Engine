//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// Exceptions.h : file containing Exception classes
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "../Helpers/stdafx.h"

struct EffectTechniqueNotFoundException
{

};

struct LoaderException
{
	LoaderException(std::tstring _text):text(_text){}

	std::tstring text;
};

struct LoaderNotFoundException
{

};

struct NoMatchingInputLayoutException
{

};