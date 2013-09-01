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

#include "../../Helpers/Namespace.h"
#include "../../Scenegraph/ObjectComponent.h"
class GameObject;

class NxActor;

enum class TriggerAction : unsigned char
{
	Enter,
	Leave
};

enum class Constraints : unsigned char{
	None			= 0,
	Freeze_PosX		= 1,
	Freeze_PosY		= 1 << 1,
	Freeze_PosZ		= 1 << 2,
	Freeze_RotX		= 1 << 3,
	Freeze_RotY		= 1 << 4,
	Freeze_RotZ		= 1 << 5,
	DisableGravity	= 1 << 6
};

inline Constraints operator|(Constraints a, Constraints b)
{return static_cast<Constraints>( static_cast<unsigned char>(a) | static_cast<unsigned char>(b) );}

inline Constraints operator&(Constraints a, Constraints b)
{return static_cast<Constraints>( static_cast<unsigned char>(a) & static_cast<unsigned char>(b) );}

inline Constraints operator~(Constraints c)
{return static_cast<Constraints>( ~static_cast<unsigned char>(c) );}


class RigidBodyComponent : public ObjectComponent
{
	typedef std::function<void (RigidBodyComponent* triggerobject, RigidBodyComponent* otherobject, TriggerAction action)> PhysicsCallback;

public:
	//constructor & destructor
	RigidBodyComponent(GameObject* pParent);
	virtual ~RigidBodyComponent(void);

	//Overrides
	virtual void Initialize(void) override;
	virtual void Update(const tt::GameContext& context) override;

	void AddShape(NxShapeDesc* pShape);

	void AddForce(tt::Vector3 force, NxForceMode mode = NX_FORCE);
	void PutToSleep();
	
	void OnTriggerEnter(RigidBodyComponent* otherObject);
	void OnTriggerLeave(RigidBodyComponent* otherObject);
	
	void SetOnTriggerCallBack(PhysicsCallback callback);

	void AddConstraints(Constraints constraintsToAdd);
	void RemoveConstraints(Constraints constraintsToRemove);

	//Accessors
	GameObject* GetParent(void) const;
	float GetMass(void) const;
	float GetAngularDamping(void) const;
	float GetDensity(void) const;
	tt::Vector3 GetLinearVelocity(void) const;
	bool IsStatic(void) const;
	NxActor* GetActor(void) const;
	void SetMass(float value);
	void SetAngularDamping(float value);
	void SetDensity(float value);
	void SetLinearVelocity(const tt::Vector3& value);
	void SetStatic(bool value);
	
private:	
	vector<NxShapeDesc*> m_Shapes;
	NxActorDesc m_ActorDesc;
	NxBodyDesc m_BodyDesc;

	float m_Mass, m_AngularDamping, m_Density;
	tt::Vector3 m_LinearVelocity, m_AngularVelocity, m_LinearMomentum, m_AngularMomentum;

	bool m_bConstraintsChanged;
	Constraints m_Constraints;

	NxActor* m_pActor;
	GameObject* m_pParentObject;

	bool m_bStatic;

	PhysicsCallback m_OnTriggerCallback;

	bool m_bInitialized;
		
	void Translate(const tt::Vector3& position);
	void Rotate(const tt::Quaternion& rotation);

	tt::Vector3 GetTranslation(void) const;
	tt::Quaternion GetRotation(void) const;

	//Disabling default copy constructor & assignment operator
	RigidBodyComponent(const RigidBodyComponent& src);
	RigidBodyComponent& operator=(const RigidBodyComponent& src);
};
