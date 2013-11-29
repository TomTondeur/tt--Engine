#pragma once

#include "../../Material.h"

class CompositeDeferredMaterial : public Material
{
public:
	//Default constructor & destructor
	CompositeDeferredMaterial(void);
	virtual ~CompositeDeferredMaterial(void);

	//Methods
	virtual void InitializeEffectVariables(void);
	virtual void UpdateEffectVariables(const tt::GameContext& context) override;

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	CompositeDeferredMaterial(const CompositeDeferredMaterial& src);
	CompositeDeferredMaterial& operator=(const CompositeDeferredMaterial& src);
};
