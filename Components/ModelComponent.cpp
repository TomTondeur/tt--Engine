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
#include "../Graphics/GraphicsDevice.h"
#include "../Graphics/Material.h"
#include "../Graphics/Materials/SkinnedMaterial.h"
#include "../Graphics/SpriteFont.h"
#include "../Components/TransformComponent.h"
#include "../AbstractGame.h"
#include "../Scenegraph/GameScene.h"
#include "CameraComponent.h"

ModelComponent::ModelComponent(std::tstring modelFilename, const TransformComponent* pTransform):m_ModelFile(modelFilename),m_pTransform(pTransform),m_pMeshAnimator(nullptr)
{

}

ModelComponent::~ModelComponent(void)
{
	delete m_pMeshAnimator;
}

//Methods

void ModelComponent::Initialize(void)
{
	m_pModel = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<Model3D>(m_ModelFile);
	
	if(m_pModel->HasAnimData()){
		m_pMeshAnimator = new MeshAnimator();
		m_pMeshAnimator->SetModel(m_pModel.get() );
		m_pMeshAnimator->SetAnimationClip(_T("WalkCycle"));
	}
}

void ModelComponent::Update(const tt::GameContext& context)
{
	if(m_pModel->HasAnimData())
		m_pMeshAnimator->Update(context);
}

void ModelComponent::Draw(const tt::GameContext& context)
{
	if(m_pMaterial == nullptr)
		throw exception();
	/*
	if( Cull(context) )
		return;	
	*/
	auto pMat = dynamic_cast<SkinnedMaterial*>(m_pMaterial.get() );
	if(pMat){
		if(!m_pModel->HasAnimData())
			MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(_T("SkinnedMaterial cannot be assigned to a model without animation data"), LogLevel::Error);
		
		m_pMeshAnimator->Draw(context);
		pMat->SetBoneTransforms(m_pMeshAnimator->GetBoneTransforms() );
		pMat->SetDualQuats(m_pMeshAnimator->GetDualQuats() );
		pMat->SetLightDirection(tt::Vector3(0,-1,0) );
	}

	MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->Draw(m_pModel, m_pTransform->GetWorldMatrix(), m_pMaterial, context);
}

void ModelComponent::DrawDeferred(const tt::GameContext& context)
{
	if(m_pMaterial == nullptr)
		throw exception();
	
	if( Cull(context) )
		return;	
	
	auto pMat = dynamic_cast<SkinnedMaterial*>(m_pMaterial.get() );
	if(pMat){
		if(!m_pModel->HasAnimData())
			MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(_T("SkinnedMaterial cannot be assigned to a model without animation data"), LogLevel::Error);
		
		m_pMeshAnimator->Draw(context);
		pMat->SetBoneTransforms(m_pMeshAnimator->GetBoneTransforms() );
		pMat->SetDualQuats(m_pMeshAnimator->GetDualQuats() );
		pMat->SetLightDirection(tt::Vector3(0,-1,0) );
	}

	MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->DrawDeferred(m_pModel, m_pTransform->GetWorldMatrix(), m_pMaterial, context);
}

void ModelComponent::GenerateShadows(const tt::GameContext& context)
{
	MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GenerateShadows(m_pModel, m_pTransform->GetWorldMatrix(), m_pMaterial, context);
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
	
	AABBox aabBox = m_pModel->GetAABB();
	aabBox.GetVertices(vertices);
	
	//If one point is inside the view frustum, the bounding volume is visible
	for(unsigned int i=0; i<8; ++i){
		vertices[i] = vertices[i].TransformPoint(wvpMat);

		if( vertices[i].x >= -1 && vertices[i].x <= 1 && 
			vertices[i].y >= -1 && vertices[i].y <= 1 &&
			vertices[i].z >= 0  && vertices[i].z <= 1 )
			return false;
	}
	
	//Additional checks to make sure we don't cull edge cases	
	tt::Matrix4x4 invWvpMat = wvpMat.Inverse();
	tt::Vector3 origin[] = {tt::Vector3(-1, -1, 0),
							tt::Vector3(-1,  1, 0),
							tt::Vector3( 1, -1, 0),
							tt::Vector3( 1,  1, 0),
							};
	tt::Vector3 destination[] = {tt::Vector3(-1, -1, 1),
								 tt::Vector3(-1,  1, 1),
								 tt::Vector3( 1, -1, 1),
								 tt::Vector3( 1,  1, 1),
								 };
	tt::Vector3 direction = tt::Vector3(0,0,1).TransformVector(invWvpMat);
	
	if(aabBox.Intersect(Ray(origin[0].TransformPoint(invWvpMat), destination[0].TransformPoint(invWvpMat)), 0, 1) )
		return false;
	if(aabBox.Intersect(Ray(origin[1].TransformPoint(invWvpMat), destination[1].TransformPoint(invWvpMat)), 0, 1) )
		return false;
	if(aabBox.Intersect(Ray(origin[2].TransformPoint(invWvpMat), destination[2].TransformPoint(invWvpMat)), 0, 1) )
		return false;
	if(aabBox.Intersect(Ray(origin[3].TransformPoint(invWvpMat), destination[3].TransformPoint(invWvpMat)), 0, 1) )
		return false;

	return true;
}