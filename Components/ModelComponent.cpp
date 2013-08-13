// Copyright © 2013 Tom Tondeur
// 
// This file is part of tt::Engine.
// 
// tt::Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// tt::Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with tt::Engine.  If not, see <http://www.gnu.org/licenses/>.

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