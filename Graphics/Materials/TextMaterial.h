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

#pragma once

#include "../Material.h"

class TextMaterial : public Material
{
public:
	//Default constructor & destructor
	TextMaterial(void);
	virtual ~TextMaterial(void);
	
	//Methods
	virtual void UpdateEffectVariables(const tt::GameContext& context);

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	TextMaterial(const TextMaterial& src);
	TextMaterial& operator=(const TextMaterial& src);
};
