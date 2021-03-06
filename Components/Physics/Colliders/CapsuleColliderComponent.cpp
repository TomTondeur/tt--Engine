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

#include "CapsuleColliderComponent.h"
#include "../RigidBodyComponent.h"

CapsuleColliderComponent::CapsuleColliderComponent(RigidBodyComponent* pRigidBody, float height, float radius) : BaseColliderComponent(pRigidBody), m_Height(height), m_Radius(radius)
{
	m_CapsuleShapeDesc.setToDefault();
	m_CapsuleShapeDesc.height = m_Height;
	m_CapsuleShapeDesc.radius= m_Radius;
	m_CapsuleShapeDesc.userData = m_pRigidBody;
}

CapsuleColliderComponent::~CapsuleColliderComponent(void)
{

}

//Methods

void CapsuleColliderComponent::Initialize(void)
{
	if(m_bIsTrigger)
		m_CapsuleShapeDesc.shapeFlags |= NX_TRIGGER_ENABLE;

	m_CapsuleShapeDesc.materialIndex = GetMaterialIndex();
	
	m_pRigidBody->AddShape(&m_CapsuleShapeDesc);
}
