#pragma once

#include "../Scenegraph/SceneObject.h"

#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"

class FreeCamera : public GenericSceneObject<TransformComponent, CameraComponent>
{
public:
	//Default constructor & destructor
	FreeCamera(void);
	virtual ~FreeCamera(void);

	//Methods
	virtual void Initialize(void) override;
	virtual void Update(const tt::GameContext& context) override;

private:
	//Datamembers
	float m_MovementSpeed, m_RotationSpeed;
	float m_Yaw, m_Pitch;

	//Disabling default copy constructor & assignment operator
	FreeCamera(const FreeCamera& src);
	FreeCamera& operator=(const FreeCamera& src);
};
