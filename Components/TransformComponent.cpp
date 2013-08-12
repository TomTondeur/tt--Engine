#include "TransformComponent.h"

using namespace tt;

TransformComponent::TransformComponent(void):m_WorldPosition(0),m_WorldRotation(Quaternion::Identity()),m_WorldScale(1),m_World(Matrix4x4::Identity())
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
	bool transformed = false;

	if(m_DeltaPosition != zeroVec){
		m_WorldPosition += m_DeltaPosition;
		m_DeltaPosition = zeroVec;
		transformed = true;
	}

	if(m_DeltaRotation != idQuat){
		m_WorldRotation *= m_DeltaRotation;
		m_DeltaRotation = idQuat;
		transformed = true;
	}

	if(m_DeltaScale != zeroVec){
		m_WorldScale += m_DeltaScale;
		m_DeltaScale = zeroVec;
		transformed = true;
	}
	
	if(!transformed && !bForce)
		return;	
	
	auto matTrans = Matrix4x4::Translation(m_WorldPosition);
	auto matRot = Matrix4x4::Rotation(m_WorldRotation);
	auto matScale = Matrix4x4::Scale(m_WorldScale);

	m_World = matScale * matRot * matTrans;
	
	m_Forward = Vector3(0,0,1).TransformCoord(matRot);
	m_Right = Vector3(1,0,0).TransformCoord(matRot);
	m_Up = m_Forward.Cross(m_Right).Normalize();
}

void TransformComponent::Translate(Vector3 translation)
{
	m_DeltaPosition += translation;
}

void TransformComponent::Translate(float x, float y, float z)
{
	Translate(Vector3(x,y,z));
}

void TransformComponent::Rotate(Quaternion rotation)
{
	m_DeltaRotation *= rotation;
}

void TransformComponent::Rotate(float x, float y, float z)
{
	Rotate(Quaternion::FromEuler(x,y,z));
}

void TransformComponent::Scale(Vector3 scale)
{
	m_DeltaScale *= scale;
}

void TransformComponent::Scale(float x, float y, float z)
{
	Scale(Vector3(x,y,z));
}

Vector3 TransformComponent::GetWorldPosition() const
{ 
	const_cast<TransformComponent*>(this)->CheckForUpdate(); 
	return m_WorldPosition;
}

Quaternion TransformComponent::GetWorldRotation() const
{ 
	const_cast<TransformComponent*>(this)->CheckForUpdate(); 
	return m_WorldRotation;
}

Vector3 TransformComponent::GetWorldScale() const
{ 
	const_cast<TransformComponent*>(this)->CheckForUpdate(); 
	return m_WorldScale; 
}

Matrix4x4 TransformComponent::GetWorldMatrix() const
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
