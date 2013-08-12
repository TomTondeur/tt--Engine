#pragma once

#include "../Helpers/Namespace.h"
#include "../Scenegraph/ObjectComponent.h"

struct CameraAttributes
{
	float fov;
	float orthoSize;
	float zNear;
	float zFar;
	bool usePerspective;
};

class TransformComponent;

class CameraComponent : public ObjectComponent
{
public:
	// Constructor & destructor
	CameraComponent(TransformComponent* pParentTransform);
	virtual ~CameraComponent(void);

	//Methods
	virtual void Update(const tt::GameContext& context);
	virtual void SetActive(bool b) override;

	const tt::Matrix4x4& GetView(void) const;
	const tt::Matrix4x4& GetProjection(void) const;
	const tt::Matrix4x4& GetViewInverse(void) const;
	CameraAttributes& GetAttributes(void);

private:
	//Datamembers
	tt::Matrix4x4 m_MatView, m_MatProj, m_MatViewInv;
	CameraAttributes m_Attributes;
	TransformComponent* m_pParentTransform;

	//Disabling default copy constructor & assignment operator
	CameraComponent(const CameraComponent& src);
	CameraComponent& operator=(const CameraComponent& src);
};
