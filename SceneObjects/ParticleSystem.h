#pragma once

#include "../Scenegraph/SceneObject.h"
#include "../Components/TransformComponent.h"
#include "../Components/ParticleEmitterComponent.h"

class ParticleSystem : public GenericSceneObject<TransformComponent, ParticleEmitterComponent>
{
public:
	//Default constructor & destructor
	ParticleSystem(void);
	virtual ~ParticleSystem(void);

	//Methods
	virtual void Initialize(void) override;
	virtual void Draw(const tt::GameContext& context) override;

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	ParticleSystem(const ParticleSystem& src);
	ParticleSystem& operator=(const ParticleSystem& src);
};
