#pragma once

class SphereColliderComponent
{
public:
	//Default constructor & destructor
	SphereColliderComponent(void);
	virtual ~SphereColliderComponent(void);

	//Methods

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	SphereColliderComponent(const SphereColliderComponent& src);
	SphereColliderComponent& operator=(const SphereColliderComponent& src);
};
