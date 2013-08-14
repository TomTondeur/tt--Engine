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

#include "TransformComponent.h"

using namespace tt;

TransformComponent::TransformComponent(void):m_WorldPosition(0),m_WorldRotation(Quaternion::Identity()),m_WorldScale(1),m_World(Matrix4x4::Identity())
											,m_NewPosition(0),m_NewRotation(Quaternion::Identity()),m_NewScale(0)
											,m_DeltaPosition(0),m_DeltaRotation(Quaternion::Identity()),m_DeltaScale(0)
											,m_Forward(0,0,1),m_Right(1,0,0),m_Up(0,1,0)
{

}

TransformComponent::~TransformComponent(void)
{

}

//Methods

void TransformComponent::Update(const tt::GameContext& context)
{
	CheckForUpdate(true);
}

void TransformComponent::CheckForUpdate(bool bForce)
{
	auto zeroVec = Vector3(0);
	auto idQuat = Quaternion::Identity();
	bool bTransformed = false;

	if(m_NewPosition != zeroVec){
		m_WorldPosition = m_NewPosition;		
		m_NewPosition = zeroVec;		
		bTransformed = true;
	}
	if(m_DeltaPosition != zeroVec){
		m_WorldPosition += m_DeltaPosition;
		m_DeltaPosition = zeroVec;
		bTransformed = true;
	}

	if(m_NewRotation != idQuat){
		m_WorldRotation = m_NewRotation;		
		m_NewRotation = idQuat;		
		bTransformed = true;
	}
	if(m_DeltaRotation != idQuat){
		m_WorldRotation *= m_DeltaRotation;
		m_DeltaRotation = idQuat;
		bTransformed = true;
	}

	if(m_NewScale != zeroVec){
		m_WorldScale = m_NewScale;
		m_NewScale = zeroVec;		
		bTransformed = true;
	}
	if(m_DeltaScale != zeroVec){
		m_WorldScale *= m_DeltaScale;
		m_DeltaScale = zeroVec;
		bTransformed = true;
	}
	
	if(!bTransformed && !bForce)
		return;	
	
	auto matTrans = Matrix4x4::Translation(m_WorldPosition);
	auto matRot = Matrix4x4::Rotation(m_WorldRotation);
	auto matScale = Matrix4x4::Scale(m_WorldScale);

	m_World = matScale * matRot * matTrans;
	
	m_Forward = Vector3(0,0,1).TransformCoord(matRot);
	m_Right = Vector3(1,0,0).TransformCoord(matRot);
	m_Up = m_Forward.Cross(m_Right).Normalize();
}

void TransformComponent::Translate(Vector3 translation, bool bRelative)
{
	if(bRelative)
		m_DeltaPosition += translation;
	else
		m_NewPosition = translation;
}

void TransformComponent::Translate(float x, float y, float z, bool bRelative)
{
	Translate(Vector3(x,y,z), bRelative);
}

void TransformComponent::Rotate(Quaternion rotation, bool bRelative)
{
	if(bRelative)
		m_DeltaRotation *= rotation;
	else
		m_NewRotation = rotation;
}

void TransformComponent::Rotate(float x, float y, float z, bool bRelative)
{
	Rotate(Quaternion::FromEuler(x,y,z), bRelative);
}

void TransformComponent::Scale(Vector3 scale, bool bRelative)
{
	if(bRelative)
		m_DeltaScale *= scale;
	else
		m_NewScale = scale;
}

void TransformComponent::Scale(float x, float y, float z, bool bRelative)
{
	Scale(Vector3(x,y,z), bRelative);
}

const Vector3& TransformComponent::GetWorldPosition() const
{ 
	const_cast<TransformComponent*>(this)->CheckForUpdate(); 
	return m_WorldPosition;
}

const Quaternion& TransformComponent::GetWorldRotation() const
{ 
	const_cast<TransformComponent*>(this)->CheckForUpdate(); 
	return m_WorldRotation;
}

const Vector3& TransformComponent::GetWorldScale() const
{ 
	const_cast<TransformComponent*>(this)->CheckForUpdate(); 
	return m_WorldScale; 
}

const Matrix4x4& TransformComponent::GetWorldMatrix() const
{ 
	const_cast<TransformComponent*>(this)->CheckForUpdate(); 
	return m_World; 
}

Vector3 TransformComponent::GetForward() const
{ 
	return m_Forward;
}

Vector3 TransformComponent::GetRight() const
{ 
	return m_Right;
}

Vector3 TransformComponent::GetUp() const
{ 
	return m_Up;
}

Vector3 TransformComponent::GetBackward() const
{ 
	return -m_Forward;
}

Vector3 TransformComponent::GetLeft() const
{ 
	return -m_Right;
}

Vector3 TransformComponent::GetDown() const
{ 
	return -m_Up;
}
