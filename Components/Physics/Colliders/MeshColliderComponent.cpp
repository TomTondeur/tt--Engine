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

#include "MeshColliderComponent.h"
#include "../RigidBodyComponent.h"
#include "../../../Services/ServiceLocator.h"

MeshColliderComponent::MeshColliderComponent(RigidBodyComponent* pRigidBody, const std::tstring& filename, MeshType meshType):BaseColliderComponent(pRigidBody),
																																m_Filename(filename),
																																m_MeshType(meshType)
{
	m_ConvexShapeDesc.setToDefault(); 
	m_TriMeshShapeDesc.setToDefault();
	m_ConvexShapeDesc.userData = m_TriMeshShapeDesc.userData = m_pRigidBody;
}

MeshColliderComponent::~MeshColliderComponent(void)
{}

//Methods

void MeshColliderComponent::Initialize(void)
{
	switch(m_MeshType){
	case MeshType::Convex:
		m_ConvexShapeDesc.meshData = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<NxConvexMesh>(m_Filename).get();

		if(m_bIsTrigger)
			m_ConvexShapeDesc.shapeFlags |= NX_TRIGGER_ENABLE;

		m_ConvexShapeDesc.materialIndex = GetMaterialIndex();
		
		m_pRigidBody->AddShape(&m_ConvexShapeDesc);
		break;
	case MeshType::Concave:
		m_TriMeshShapeDesc.meshData = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<NxTriangleMesh>(m_Filename).get();

		if(m_bIsTrigger)
			m_TriMeshShapeDesc.shapeFlags |= NX_TRIGGER_ENABLE;

		m_TriMeshShapeDesc.materialIndex = GetMaterialIndex();
		
		m_pRigidBody->AddShape(&m_TriMeshShapeDesc);
		break;
	}
}
