#pragma once

class BoxColliderComponent
{
public:
	//Default constructor & destructor
	BoxColliderComponent(void);
	virtual ~BoxColliderComponent(void);

	//Methods

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	BoxColliderComponent(const BoxColliderComponent& src);
	BoxColliderComponent& operator=(const BoxColliderComponent& src);
};
