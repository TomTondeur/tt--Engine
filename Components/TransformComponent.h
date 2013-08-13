#pragma once

#include "../Helpers/Namespace.h"
#include "../Helpers/D3DUtil.h"
#include "../Scenegraph/SceneObject.h"

class TransformComponent final : public ObjectComponent
{
public:
	//Constructors & destructor
	TransformComponent(void);
	TransformComponent(SceneObject* parent);
	~TransformComponent(void);

	//Methods
	void Update(const tt::GameContext& context) override;

	void Translate(tt::Vector3 translation, bool bRelative=false);
	void Translate(float x, float y, float z, bool bRelative=false);

	void Rotate(tt::Quaternion rotation, bool bRelative=false);
	void Rotate(float x, float y, float z, bool bRelative=false);

	void Scale(tt::Vector3 scale, bool bRelative=false);
	void Scale(float x, float y, float z, bool bRelative=false);

	//Accessors
	tt::Vector3 GetWorldPosition() const;
	tt::Quaternion GetWorldRotation() const;
	tt::Vector3 GetWorldScale() const;
	tt::Matrix4x4 GetWorldMatrix() const;

	tt::Vector3 GetForward() const;
	tt::Vector3 GetRight() const;
	tt::Vector3 GetUp() const;
	tt::Vector3 GetBackward() const;
	tt::Vector3 GetLeft() const;
	tt::Vector3 GetDown() const;

private:
	//Datamembers
	tt::Vector3		m_WorldPosition, m_NewPosition, m_DeltaPosition;
	tt::Quaternion	m_WorldRotation, m_NewRotation, m_DeltaRotation;
	tt::Vector3		m_WorldScale,	 m_NewScale,	m_DeltaScale;
	tt::Matrix4x4 m_World;

	tt::Vector3 m_Forward, m_Right, m_Up;

	//Internal methods
	void CheckForUpdate(bool bForce = false);

	//Disabling default copy constructor & assignment operator
	TransformComponent(const TransformComponent& src);
	TransformComponent& operator=(const TransformComponent& src);
};
