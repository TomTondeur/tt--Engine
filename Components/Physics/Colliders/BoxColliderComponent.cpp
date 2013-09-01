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

#include "BoxColliderComponent.h"
#include "../RigidBodyComponent.h"

BoxColliderComponent::BoxColliderComponent(RigidBodyComponent* pRigidBody, const tt::Vector3& dimensions):BaseColliderComponent(pRigidBody), m_Dimensions(dimensions)
{
	m_BoxShapeDesc.setToDefault();
	m_BoxShapeDesc.dimensions = static_cast<NxVec3>(m_Dimensions * .5f);
	m_BoxShapeDesc.userData = pRigidBody;
}

BoxColliderComponent::~BoxColliderComponent(void)
{

}

//Methods

void BoxColliderComponent::Initialize(void)
{
	if(m_bIsTrigger)
		m_BoxShapeDesc.shapeFlags |= NX_TRIGGER_ENABLE;

	m_BoxShapeDesc.materialIndex = GetMaterialIndex();
	
	m_pRigidBody->AddShape(&m_BoxShapeDesc);
}
