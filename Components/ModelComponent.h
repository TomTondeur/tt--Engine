#pragma once

#include "../Scenegraph/ObjectComponent.h"
#include "../Helpers/Namespace.h"
#include "../Helpers/resrc_ptr.hpp"

class Material;
class Model3D;
class TransformComponent;

class ModelComponent : public ObjectComponent
{
public:
	//Constructor & destructor
	ModelComponent(std::tstring modelFilename, const TransformComponent* pTransform);
	virtual ~ModelComponent(void);

	//Methods
	//virtual void Update(const tt::GameContext& context) override;
	virtual void Draw(const tt::GameContext& context) override;
	virtual void Initialize(void) override;

	void SetMaterial(resource_ptr<Material> pMat);
	const TransformComponent* GetTransform(void) const;

private:
	//Datamembers
	std::tstring m_ModelFile;
	resource_ptr<Model3D> m_pModel;
	resource_ptr<Material> m_pMaterial;
	const TransformComponent* m_pTransform;

	//Disabling default copy constructor & assignment operator
	ModelComponent(const ModelComponent& src);
	ModelComponent& operator=(const ModelComponent& src);
};
