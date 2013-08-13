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

#include "../Helpers/Namespace.h"
#include "../Scenegraph/ObjectComponent.h"

struct CameraAttributes
{
	float fov;
	float orthoSize;
	float zNear;
	float zFar;
	bool usePerspective;
};

class TransformComponent;

class CameraComponent : public ObjectComponent
{
public:
	// Constructor & destructor
	CameraComponent(TransformComponent* pParentTransform);
	virtual ~CameraComponent(void);

	//Methods
	virtual void Update(const tt::GameContext& context);
	virtual void SetActive(bool b) override;

	const tt::Matrix4x4& GetView(void) const;
	const tt::Matrix4x4& GetProjection(void) const;
	const tt::Matrix4x4& GetViewInverse(void) const;
	CameraAttributes& GetAttributes(void);

private:
	//Datamembers
	tt::Matrix4x4 m_MatView, m_MatProj, m_MatViewInv;
	CameraAttributes m_Attributes;
	TransformComponent* m_pParentTransform;

	//Disabling default copy constructor & assignment operator
	CameraComponent(const CameraComponent& src);
	CameraComponent& operator=(const CameraComponent& src);
};
