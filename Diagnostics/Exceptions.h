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
	LoaderException(const std::tstring& _text):text(_text){}

	LoaderException(const std::tstring& _text, const std::tstring& _errors):text(_text), errors(_errors){}

	std::tstring text;
	std::tstring errors;
};

struct LoaderNotFoundException
{

};

struct NoMatchingInputLayoutException
{

};