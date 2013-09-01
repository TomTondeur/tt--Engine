#pragma once

#include "NxMaterialDesc.h"
#include "../../Helpers/Namespace.h"

class NxMaterial;

class PhysicsMaterial
{
public:
	//Default constructor & destructor
	PhysicsMaterial();
	virtual ~PhysicsMaterial();

	void Initialize(void);

	void SetDynamicFriction(float dynamicFriction);
	void SetStaticFriction(float staticFriction);
	void SetRestitution(float restitution);

	void SetDynamicFrictionV(float dynamicFrictionV);
	void SetStaticFrictionV(float staticFrictionV);

	void SetDirectionOfAnisotropy(const tt::Vector3& dirOfAnisotropy);
	void SetFrictionCombineMode(NxCombineMode frictionCombineMode);
	void SetRestitutionCombineMode(NxCombineMode restitutionCombineMode);

	float GetDynamicFriction(void) const;
	float GetStaticFriction(void) const;
	float GetRestitution(void) const;
		  
	float GetDynamicFrictionV(void) const;
	float GetStaticFrictionV(void) const;

	tt::Vector3 GetDirectionOfAnisotropy(void) const;
	NxCombineMode GetFrictionCombineMode(void) const;
	NxCombineMode GetRestitutionCombineMode(void) const;

	NxMaterial* GetMaterial(void) const;
	bool IsInitialized(void) const;

private:
	//Datamembers
	bool m_bIsInitialized;
	NxMaterialDesc m_MaterialDesc;
	NxMaterial* m_pPhysicsMaterial;

	//Disabling default copy constructor & assignment operator
	PhysicsMaterial(const PhysicsMaterial& src);
	PhysicsMaterial& operator=(const PhysicsMaterial& src);
};
