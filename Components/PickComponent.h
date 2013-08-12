#pragma once

class PickComponent
{
public:
	//Default constructor & destructor
	PickComponent(void);
	virtual ~PickComponent(void);

	//Methods

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	PickComponent(const PickComponent& src);
	PickComponent& operator=(const PickComponent& src);
};
