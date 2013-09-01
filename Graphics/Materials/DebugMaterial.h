#pragma once

#include "../Material.h"

class DebugMaterial : public Material
{
public:
	//Default constructor & destructor
	DebugMaterial(void);
	virtual ~DebugMaterial(void);

	//Methods
	virtual void UpdateEffectVariables(const tt::GameContext& context) override;

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	DebugMaterial(const DebugMaterial& src);
	DebugMaterial& operator=(const DebugMaterial& src);
};
