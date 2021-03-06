// Copyright � 2013 Tom Tondeur
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

#include "BaseColliderComponent.h"

class BoxColliderComponent : public BaseColliderComponent
{
public:
	//Default constructor & destructor
	BoxColliderComponent(RigidBodyComponent* pRigidBody, const tt::Vector3& dimensions);
	virtual ~BoxColliderComponent(void);

	//Methods
	virtual void Initialize(void) override;

private:
	//Datamembers
	NxBoxShapeDesc m_BoxShapeDesc;
	tt::Vector3 m_Dimensions;

	//Disabling default copy constructor & assignment operator
	BoxColliderComponent(const BoxColliderComponent& src);
	BoxColliderComponent& operator=(const BoxColliderComponent& src);
};
