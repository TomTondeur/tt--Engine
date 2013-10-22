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

#include "Object3DMaterial.h"
#include "../../Services/ServiceLocator.h"

Object3DMaterial::Object3DMaterial(void):Material(_T("Resources/PosNormTex.fx"))
{

}

Object3DMaterial::~Object3DMaterial(void)
{

}

//Methods

void Object3DMaterial::UpdateEffectVariables(const tt::GameContext& context)
{
	SetVariable(_T("DIRECTION"), tt::Vector3(0,-1,0));
	SetVariable(_T("TEXTURE_DIFFUSE"), MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ID3D10ShaderResourceView>(_T("Resources/Textures/TEX_Char_Goblin_Body_dif.png")).get());
}