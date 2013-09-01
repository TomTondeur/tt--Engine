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

#include "RigidBodyComponent.h"
#include "../../Services/ServiceLocator.h"

RigidBodyComponent::RigidBodyComponent(GameObject* pParent) : 
					m_Mass(0), m_AngularDamping(0), m_Density(0),
					m_LinearVelocity(0), m_AngularVelocity(0), m_LinearMomentum(0), m_AngularMomentum(0),
					m_bConstraintsChanged(false),
					m_Constraints(Constraints::None),
					m_pActor(nullptr),
					m_bStatic(false),
					m_bInitialized(false),
					m_pParentObject(pParent)
{}

RigidBodyComponent::~RigidBodyComponent(void)
{

}

void RigidBodyComponent::Initialize(void)
{
	if(m_Shapes.empty())
		return;

	NxScene* pScene = MyServiceLocator::GetInstance()->GetService<IPhysicsService>()->GetActiveScene();

	//release actor 
	if(m_pActor) 
		pScene->releaseActor(*m_pActor);

	// add shapes
	m_ActorDesc.shapes.clear();
	for(auto shapeDesc:m_Shapes)
		m_ActorDesc.shapes.pushBack(shapeDesc);

	//Create body
	if(!m_bStatic){
		m_BodyDesc.setToDefault();
		m_BodyDesc.angularDamping = m_AngularDamping;
		m_BodyDesc.linearVelocity = m_LinearVelocity;
		m_BodyDesc.mass = m_Mass;
		m_ActorDesc.body = &m_BodyDesc;
	}
	else 
		m_ActorDesc.body = 0;

	m_ActorDesc.density = m_Density;

	//set actor pose
	NxMat34 mat;
	D3DXMATRIX worldMatrix;
	D3DXMatrixIdentity(&worldMatrix);
	mat.setColumnMajor44(worldMatrix);
	m_ActorDesc.globalPose = mat;

	//Create actor
	m_pActor = pScene->createActor(m_ActorDesc);
	if(!m_pActor)
		MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(_T("Error creating actor"), LogLevel::Error);

	m_pActor->userData = this;
	m_bInitialized = true;
}

void RigidBodyComponent::Update(const tt::GameContext& context)
{
	if(!m_pActor)
		return;

	if(!m_bStatic){
		m_LinearVelocity	= static_cast<tt::Vector3>( m_pActor->getLinearVelocity()  );
		m_AngularVelocity	= static_cast<tt::Vector3>( m_pActor->getAngularVelocity() );
		m_LinearMomentum	= static_cast<tt::Vector3>( m_pActor->getLinearMomentum()  );
		m_AngularMomentum	= static_cast<tt::Vector3>( m_pActor->getAngularMomentum() );
	}

	if(!m_bConstraintsChanged)
		return;

	if( (m_Constraints & Constraints::Freeze_PosX) != Constraints::None )
		m_pActor->raiseBodyFlag(NX_BF_FROZEN_POS_X);
	else
		m_pActor->clearBodyFlag(NX_BF_FROZEN_POS_X);

	if( (m_Constraints & Constraints::Freeze_PosY) != Constraints::None )
		m_pActor->raiseBodyFlag(NX_BF_FROZEN_POS_Y);
	else
		m_pActor->clearBodyFlag(NX_BF_FROZEN_POS_Y);

	if( (m_Constraints & Constraints::Freeze_PosZ) != Constraints::None )
		m_pActor->raiseBodyFlag(NX_BF_FROZEN_POS_Z);
	else
		m_pActor->clearBodyFlag(NX_BF_FROZEN_POS_Z);

	if( (m_Constraints & Constraints::Freeze_RotX) != Constraints::None )
		m_pActor->raiseBodyFlag(NX_BF_FROZEN_ROT_X);
	else
		m_pActor->clearBodyFlag(NX_BF_FROZEN_ROT_X);

	if( (m_Constraints & Constraints::Freeze_RotY) != Constraints::None )
		m_pActor->raiseBodyFlag(NX_BF_FROZEN_ROT_Y);
	else
		m_pActor->clearBodyFlag(NX_BF_FROZEN_ROT_Y);

	if( (m_Constraints & Constraints::Freeze_RotZ) != Constraints::None )
		m_pActor->raiseBodyFlag(NX_BF_FROZEN_ROT_Z);
	else
		m_pActor->clearBodyFlag(NX_BF_FROZEN_ROT_Z);

	if( (m_Constraints & Constraints::DisableGravity) != Constraints::None )
		m_pActor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
	else
		m_pActor->clearBodyFlag(NX_BF_DISABLE_GRAVITY);

	if(m_pActor->isSleeping())
		m_pActor->wakeUp();

	m_bConstraintsChanged = false;
}

void RigidBodyComponent::AddShape(NxShapeDesc* pShape)
{
	m_Shapes.push_back(pShape);
	// If already Initialized, recreate actor
	if(m_bInitialized)
		Initialize();
}

void RigidBodyComponent::AddForce(tt::Vector3 force, NxForceMode mode)
{
	if(m_pActor != nullptr)
		m_pActor->addForce(static_cast<NxVec3>(force), mode);
}

void RigidBodyComponent::PutToSleep()
{
	if(m_pActor != nullptr)
		m_pActor->putToSleep();
}
	
void RigidBodyComponent::OnTriggerEnter(RigidBodyComponent* otherObject)
{
	if(m_OnTriggerCallback)
		m_OnTriggerCallback(this, otherObject, TriggerAction::Enter);
}

void RigidBodyComponent::OnTriggerLeave(RigidBodyComponent* otherObject)
{
	if(m_OnTriggerCallback)
		m_OnTriggerCallback(this, otherObject, TriggerAction::Leave);
}

void RigidBodyComponent::SetOnTriggerCallBack(PhysicsCallback callback)
{
	m_OnTriggerCallback = callback;
}

void RigidBodyComponent::AddConstraints(Constraints constraints)
{
	m_Constraints = m_Constraints | constraints;
	m_bConstraintsChanged = true;
}

void RigidBodyComponent::RemoveConstraints(Constraints constraints)
{
	m_Constraints = m_Constraints & ~constraints;
	m_bConstraintsChanged = true;
}

//Accessors
GameObject* RigidBodyComponent::GetParent(void) const
{
	return m_pParentObject;
}

float RigidBodyComponent::GetMass(void) const
{
	return m_Mass;
}

float RigidBodyComponent::GetAngularDamping(void) const
{
	return m_AngularDamping;
}

float RigidBodyComponent::GetDensity(void) const
{
	return m_Density;
}

tt::Vector3 RigidBodyComponent::GetLinearVelocity(void) const
{
	return m_LinearVelocity;
}

bool RigidBodyComponent::IsStatic(void) const
{
	return m_bStatic;
}

NxActor* RigidBodyComponent::GetActor(void) const
{
	return m_pActor;
}

void RigidBodyComponent::SetMass(float value)
{
	m_Mass = value;
}

void RigidBodyComponent::SetAngularDamping(float value)
{
	m_AngularDamping = value;
}

void RigidBodyComponent::SetDensity(float value)
{
	m_Density = value;
}

void RigidBodyComponent::SetLinearVelocity(const tt::Vector3& value)
{
	m_LinearVelocity = value;
}

void RigidBodyComponent::SetStatic(bool value)
{
	m_bStatic = value;
}

void RigidBodyComponent::Translate(const tt::Vector3& position)
{
	if(m_pActor)
		m_pActor->setGlobalPosition(static_cast<NxVec3>(position));
}

void RigidBodyComponent::Rotate(const tt::Quaternion& rotation)
{
	if(m_pActor)
		m_pActor->setGlobalOrientationQuat(static_cast<NxQuat>(rotation));
}

tt::Vector3	RigidBodyComponent::GetTranslation(void) const
{
	if(!m_pActor)
		return tt::Vector3(0);
	
	auto position = m_pActor->getGlobalPosition();
	return tt::Vector3(position);
}

tt::Quaternion RigidBodyComponent::GetRotation(void) const
{
	if(!m_pActor)
		return tt::Quaternion::Identity;

	auto rotation = m_pActor->getGlobalOrientationQuat();
	return tt::Quaternion(rotation);
}
