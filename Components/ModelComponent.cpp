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
#include "../AbstractGame.h"
#include "../Scenegraph/GameScene.h"
#include "CameraComponent.h"

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

	if( Cull(context) )
		return;

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

bool ModelComponent::Cull(const tt::GameContext& context)
{
	tt::Vector3 vertices[8];
	tt::Matrix4x4 wvpMat = m_pTransform->GetWorldMatrix() * context.pGame->GetActiveScene()->GetActiveCamera()->GetView() * context.pGame->GetActiveScene()->GetActiveCamera()->GetProjection();
	
	AABBox wvpBox;
	wvpBox.Bounds[0] = m_pModel->GetAABB().Bounds[0].TransformCoord(wvpMat);
	wvpBox.Bounds[1] = m_pModel->GetAABB().Bounds[1].TransformCoord(wvpMat);
	wvpBox.GetVertices(vertices);

	//If one point is inside the view frustum, the bounding volume is visible
	for(unsigned int i=0; i<8; ++i){
		vertices[i] = vertices[i].TransformCoord(wvpMat);

		if( vertices[i].x < -1 || vertices[i].x > 1 || 
			vertices[i].y < -1 || vertices[i].y > 1 ||
			vertices[i].z < 0  || vertices[i].z > 1)
			return false;
	}

	//Additional checks to make sure we don't cull edge cases	
	return	wvpBox.Intersect(AABBox::FrustumCullRay0, 0, 1) ||	
			wvpBox.Intersect(AABBox::FrustumCullRay1, 0, 1) ||
			wvpBox.Intersect(AABBox::FrustumCullRay2, 0, 1) ||
			wvpBox.Intersect(AABBox::FrustumCullRay3, 0, 1);
}