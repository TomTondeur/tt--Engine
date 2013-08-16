#pragma once

#include "../../Material.h"

class BlurMaterial : public Material
{
public:
	//Default constructor & destructor
	BlurMaterial(void);
	virtual ~BlurMaterial(void);

	//Methods
	virtual void UpdateEffectVariables(const tt::GameContext& context) override;

private:
	//Datamembers
	float m_BlurOffset;

	//Disabling default copy constructor & assignment operator
	BlurMaterial(const BlurMaterial& src);
	BlurMaterial& operator=(const BlurMaterial& src);
};
