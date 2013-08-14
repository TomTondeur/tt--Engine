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
#include "../Helpers/D3DUtil.h"
#include "../Scenegraph/SceneObject.h"

class TransformComponent final : public ObjectComponent
{
public:
	//Constructors & destructor
	TransformComponent(void);
	TransformComponent(SceneObject* parent);
	~TransformComponent(void);

	//Methods
	void Update(const tt::GameContext& context) override;

	void Translate(tt::Vector3 translation, bool bRelative=false);
	void Translate(float x, float y, float z, bool bRelative=false);

	void Rotate(tt::Quaternion rotation, bool bRelative=false);
	void Rotate(float x, float y, float z, bool bRelative=false);

	void Scale(tt::Vector3 scale, bool bRelative=false);
	void Scale(float x, float y, float z, bool bRelative=false);

	//Accessors
	const tt::Vector3&		GetWorldPosition(void) const;
	const tt::Quaternion&	GetWorldRotation(void) const;
	const tt::Vector3&		GetWorldScale(void) const;
	const tt::Matrix4x4&	GetWorldMatrix(void) const;

	tt::Vector3 GetForward(void) const;
	tt::Vector3 GetRight(void) const;
	tt::Vector3 GetUp(void) const;
	tt::Vector3 GetBackward(void) const;
	tt::Vector3 GetLeft(void) const;
	tt::Vector3 GetDown(void) const;

private:
	//Datamembers
	tt::Vector3		m_WorldPosition, m_NewPosition, m_DeltaPosition;
	tt::Quaternion	m_WorldRotation, m_NewRotation, m_DeltaRotation;
	tt::Vector3		m_WorldScale,	 m_NewScale,	m_DeltaScale;
	tt::Matrix4x4 m_World;

	tt::Vector3 m_Forward, m_Right, m_Up;

	//Internal methods
	void CheckForUpdate(bool bForce = false);

	//Disabling default copy constructor & assignment operator
	TransformComponent(const TransformComponent& src);
	TransformComponent& operator=(const TransformComponent& src);
};
