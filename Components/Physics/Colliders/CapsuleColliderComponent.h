#pragma once

class CapsuleColliderComponent
{
public:
	//Default constructor & destructor
	CapsuleColliderComponent(void);
	virtual ~CapsuleColliderComponent(void);

	//Methods

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	CapsuleColliderComponent(const CapsuleColliderComponent& src);
	CapsuleColliderComponent& operator=(const CapsuleColliderComponent& src);
};
