#pragma once

class MeshColliderComponent
{
public:
	//Default constructor & destructor
	MeshColliderComponent(void);
	virtual ~MeshColliderComponent(void);

	//Methods

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	MeshColliderComponent(const MeshColliderComponent& src);
	MeshColliderComponent& operator=(const MeshColliderComponent& src);
};
