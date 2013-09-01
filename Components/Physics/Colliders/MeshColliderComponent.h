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

#include "BaseColliderComponent.h"

enum class MeshType : unsigned char{
	Convex = 0,
	Concave = 1
};

class MeshColliderComponent : public BaseColliderComponent
{
public:
	//Default constructor & destructor
	MeshColliderComponent(RigidBodyComponent* pRigidBody, const std::tstring& filename, MeshType meshType);
	virtual ~MeshColliderComponent(void);

	//Methods
	virtual void Initialize(void) override;

private:
	//Datamembers
	std::tstring m_Filename;
	MeshType m_MeshType;
	NxConvexShapeDesc m_ConvexShapeDesc;
	NxTriangleMeshShapeDesc m_TriMeshShapeDesc;

	//Disabling default copy constructor & assignment operator
	MeshColliderComponent(const MeshColliderComponent& src);
	MeshColliderComponent& operator=(const MeshColliderComponent& src);
};
