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

#include "PlaneColliderComponent.h"
#include "../RigidBodyComponent.h"

PlaneColliderComponent::PlaneColliderComponent(RigidBodyComponent* pRigidBody, float distance, const tt::Vector3& normal):BaseColliderComponent(pRigidBody), 
																															m_Distance(distance), 
																															m_Normal(normal)
{
	m_PlaneShapeDesc.setToDefault();
	m_PlaneShapeDesc.d = m_Distance;
	m_PlaneShapeDesc.normal = m_Normal;
	m_PlaneShapeDesc.userData = m_pRigidBody;
}

PlaneColliderComponent::~PlaneColliderComponent(void)
{

}

//Methods

void PlaneColliderComponent::Initialize(void)
{
	if(m_bIsTrigger)
		m_PlaneShapeDesc.shapeFlags |= NX_TRIGGER_ENABLE;

	m_PlaneShapeDesc.materialIndex = GetMaterialIndex();

	m_pRigidBody->AddShape(&m_PlaneShapeDesc);
}
