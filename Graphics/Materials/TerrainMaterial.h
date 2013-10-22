#pragma once

#include "../Material.h"

class TerrainMaterial : public Material
{
public:
	//constructor & destructor
	TerrainMaterial(unsigned char tessellation, const std::tstring& heightMapPath);
	virtual ~TerrainMaterial(void);

	//Methods
	virtual void InitializeEffectVariables(void) override;
	virtual void UpdateEffectVariables(const tt::GameContext& context) override;

	virtual void DrawTerrain(const tt::GameContext& context, const tt::Matrix4x4& worldMat);

private:
	//Datamembers
	class Implementation;
	Implementation* m_pImpl;

	//Disabling default copy constructor & assignment operator
	TerrainMaterial(const TerrainMaterial& src);
	TerrainMaterial& operator=(const TerrainMaterial& src);
};
