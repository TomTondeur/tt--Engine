#pragma once

class RigidBodyComponent
{
public:
	//Default constructor & destructor
	RigidBodyComponent(void);
	virtual ~RigidBodyComponent(void);

	//Methods
	void OnTriggerEnter(RigidBodyComponent* pOtherObject);
	void OnTriggerLeave(RigidBodyComponent* pOtherObject);

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	RigidBodyComponent(const RigidBodyComponent& src);
	RigidBodyComponent& operator=(const RigidBodyComponent& src);
};
