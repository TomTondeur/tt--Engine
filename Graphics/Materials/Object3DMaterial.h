#pragma once

#include "../Material.h"

class Object3DMaterial : public Material
{
public:
	//Default constructor & destructor
	Object3DMaterial(void);
	virtual ~Object3DMaterial(void);

	//Methods
	virtual void UpdateEffectVariables(const tt::GameContext& context) override;

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	Object3DMaterial(const Object3DMaterial& src);
	Object3DMaterial& operator=(const Object3DMaterial& src);
};
