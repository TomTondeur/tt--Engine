#include "ModelComponent.h"
#include "../Services/ServiceLocator.h"
#include "../Graphics/Model3D.h"
//#include "../Graphics/Material.h"
#include "../Graphics/GraphicsDevice.h"
#include "../Graphics/Material.h"
#include "../Components/TransformComponent.h"

ModelComponent::ModelComponent(std::tstring modelFilename, const TransformComponent* pTransform):m_ModelFile(modelFilename),m_pTransform(pTransform)
{

}

ModelComponent::~ModelComponent(void)
{

}

//Methods

void ModelComponent::Initialize(void)
{
	m_pModel = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<Model3D>(m_ModelFile);
}

void ModelComponent::Draw(const tt::GameContext& context)
{
	if(m_pMaterial == nullptr)
		throw exception();

	MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->Draw(m_pModel, m_pTransform->GetWorldMatrix(), m_pMaterial, context);
}

void ModelComponent::SetMaterial(resource_ptr<Material> pMat)
{
	m_pMaterial = pMat;
}

const TransformComponent* ModelComponent::GetTransform(void) const
{
	return m_pTransform;
}