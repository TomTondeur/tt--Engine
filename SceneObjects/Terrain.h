#pragma once

#include "../Scenegraph/SceneObject.h"
#include "../Components/TransformComponent.h"
#include "../Components/ModelComponent.h"

class TerrainMaterial;

class Terrain : public GenericSceneObject<TransformComponent>
{
public:
	//Default constructor & destructor
	Terrain(const tt::Vector3& dimensions, unsigned int tessellation, const std::tstring& heightMapPath);
	virtual ~Terrain(void);

	//Methods
	virtual void Initialize(void) override;
	virtual void Draw(const tt::GameContext& context) override;

private:
	//Datamembers
	unsigned int m_Tessellation;
	tt::Vector3 m_Dimensions;
	TerrainMaterial* m_pMaterial;
	std::tstring m_HeightMapPath;

	//Disabling default copy constructor & assignment operator
	Terrain(const Terrain& src);
	Terrain& operator=(const Terrain& src);
};
