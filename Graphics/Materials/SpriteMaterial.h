#pragma once

#include "../Material.h"

class SpriteMaterial : public Material
{
public:
	//Default constructor & destructor
	SpriteMaterial(void);
	virtual ~SpriteMaterial(void);

	//Methods
	virtual void UpdateEffectVariables(const tt::GameContext& context);

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	SpriteMaterial(const SpriteMaterial& src);
	SpriteMaterial& operator=(const SpriteMaterial& src);
};
