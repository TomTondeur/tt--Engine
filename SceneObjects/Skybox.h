#pragma once

#include "../Scenegraph/SceneObject.h"
#include "../Components/ModelComponent.h"
#include "../Components/TransformComponent.h"

class Skybox : public GenericSceneObject<TransformComponent, ModelComponent>
{
public:
	//Default constructor & destructor
	Skybox(void);
	virtual ~Skybox(void);

	//Methods
	virtual void Initialize(void) override;
	virtual void Draw(const tt::GameContext& context) override;

private:
	//Datamembers
	std::unique_ptr<Material> m_pMaterial;

	//Disabling default copy constructor & assignment operator
	Skybox(const Skybox& src);
	Skybox& operator=(const Skybox& src);
};
