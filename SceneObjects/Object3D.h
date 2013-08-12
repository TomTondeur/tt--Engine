#pragma once

#include "../Scenegraph/SceneObject.h"

#include "../Components/TransformComponent.h"
#include "../Components/ModelComponent.h"

class Object3D : public GenericSceneObject<TransformComponent, ModelComponent>
{
public:
	//Default constructor & destructor
	Object3D(void);
	virtual ~Object3D(void);

	//Methods
	virtual void Initialize(void) override;
	virtual void Update(const tt::GameContext& context) override;

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	Object3D(const Object3D& src);
	Object3D& operator=(const Object3D& src);
};
