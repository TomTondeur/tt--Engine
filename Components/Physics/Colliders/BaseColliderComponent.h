#pragma once

#include "../../../Scenegraph/ObjectComponent.h"

class PhysicsMaterial;
class RigidBodyComponent;

class BaseColliderComponent : public ObjectComponent
{
public:
	//Default constructor & destructor
	BaseColliderComponent(RigidBodyComponent* pRigidBody);
	virtual ~BaseColliderComponent(void);

	//Methods
	
	void SetPhysicsMaterial(PhysicsMaterial* physicsMaterial);
	PhysicsMaterial* GetPhysicsMaterial() const;

	bool IsTrigger(void) const;
	void SetAsTrigger(bool value);

protected:
	//Datamembers
	bool m_bIsTrigger;
	PhysicsMaterial* m_pPhysicsMaterial;
	RigidBodyComponent* m_pRigidBody;
	
	unsigned int GetMaterialIndex(void);

private:
	//Disabling default copy constructor & assignment operator
	BaseColliderComponent(const BaseColliderComponent& src);
	BaseColliderComponent& operator=(const BaseColliderComponent& src);
};
