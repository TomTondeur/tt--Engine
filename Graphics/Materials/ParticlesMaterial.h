#pragma once

#include "../Material.h"
#include "../../Helpers/resrc_ptr.hpp"

class ParticlesMaterial : public Material
{
public:
	//Default constructor & destructor
	ParticlesMaterial(void);
	virtual ~ParticlesMaterial(void);

	//Methods
	virtual void UpdateEffectVariables(const tt::GameContext& context) override;
	virtual void InitializeEffectVariables(void) override;

private:
	//Datamembers
	resource_ptr<ID3D10ShaderResourceView> m_pTextureDiffuse;
	ID3D10ShaderResourceView* m_pTextureRandom;

	void CreateRandomTexture(void);

	//Disabling default copy constructor & assignment operator
	ParticlesMaterial(const ParticlesMaterial& src);
	ParticlesMaterial& operator=(const ParticlesMaterial& src);
};
