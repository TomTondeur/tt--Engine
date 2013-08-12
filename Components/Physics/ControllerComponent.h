#pragma once

class ControllerComponent
{
public:
	//Default constructor & destructor
	ControllerComponent(void);
	virtual ~ControllerComponent(void);

	//Methods

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	ControllerComponent(const ControllerComponent& src);
	ControllerComponent& operator=(const ControllerComponent& src);
};
