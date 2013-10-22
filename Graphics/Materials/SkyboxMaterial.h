#pragma once

#include "../Material.h"

class SkyboxMaterial : public Material
{
public:
	//Default constructor & destructor
	SkyboxMaterial(const std::tstring& cubeMapFilename);
	virtual ~SkyboxMaterial(void);

	//Methods
	virtual void UpdateEffectVariables(const tt::GameContext& context) override;
	virtual void InitializeEffectVariables(void) override;

private:
	//Datamembers
	resource_ptr<ID3D10ShaderResourceView> m_pTexture;
	std::tstring m_CubemapFilename;


	//Disabling default copy constructor & assignment operator
	SkyboxMaterial(const SkyboxMaterial& src);
	SkyboxMaterial& operator=(const SkyboxMaterial& src);
};
