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

#include "GameScene.h"
#include "../Components/CameraComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ParticleEmitterComponent.h"
#include "../Graphics/PostProcessingEffect.h"
#include "../Graphics/GraphicsDevice.h"
#include "../Graphics/RenderTarget2D.h"
#include "../Graphics/SpriteBatch.h"
#include "../Services/ServiceLocator.h"

GameScene* GameScene::s_pActiveScene = nullptr;

GameScene::GameScene():m_pPhysicsScene(nullptr), m_pActiveCamera(nullptr){}
GameScene::~GameScene()
{
	for(auto pObj : m_Objects)
		delete pObj;

	std::set<PostProcessingEffect*> uniquePostproEffects;
	for(auto effectPair : m_PostProEffects)
		uniquePostproEffects.insert(effectPair.second);

	for(auto pEffect : uniquePostproEffects)
		delete pEffect;

	NxGetPhysicsSDK()->releaseScene(*m_pPhysicsScene);
}
	
void GameScene::InitializeScene(void)
{
	m_pPhysicsScene = MyServiceLocator::GetInstance()->GetService<IPhysicsService>()->GetActiveScene();

	for(auto pObj : m_Objects){
		pObj->Initialize();
		pObj->InitializeObject();
	}

	for(auto effectPair : m_PostProEffects)
		effectPair.second->Initialize();
}

void GameScene::UpdateScene(const tt::GameContext& context)
{
	for(auto pObj : m_Objects){
		pObj->Update(context);
		pObj->UpdateObject(context);
	}
}

void GameScene::DrawScene(const tt::GameContext& context)
{
	for(auto pObj : m_Objects){
		pObj->Draw(context);
		pObj->DrawObject(context);
	}

	auto pGfxService = MyServiceLocator::GetInstance()->GetService<IGraphicsService>();
	
	pGfxService->GetSpriteBatch()->Flush(context);
	
	auto particlesSprite = ParticleEmitterComponent::RenderDeferred(context);
	//pGfxService->GetSpriteBatch()->Draw(particlesSprite);
	//pGfxService->GetSpriteBatch()->Flush(context);
	
	if(!m_PostProEffects.empty() ){
		auto postProSprite = pGfxService->RenderPostProcessing(context, m_PostProEffects);

		pGfxService->GetSpriteBatch()->Draw(postProSprite);
		pGfxService->GetSpriteBatch()->Flush(context);
	}
}
	
void GameScene::Initialize(void){}
void GameScene::Update(const tt::GameContext& context){}
void GameScene::Draw(const tt::GameContext& context){}

std::tstring GameScene::GetName(void) const
{
	return m_Name;
}

NxScene* GameScene::GetPhysicsScene(void) const
{
	return m_pPhysicsScene;
}

GameScene* GameScene::GetActiveScene(void)
{
	return s_pActiveScene;
}

void GameScene::SetActiveCamera(CameraComponent* pCam)
{
	if(m_pActiveCamera)
		m_pActiveCamera->SetActive(false);
	
	m_pActiveCamera = pCam;
}

const CameraComponent* GameScene::GetActiveCamera(void) const
{
	return m_pActiveCamera;
}

void GameScene::AddSceneObject(SceneObject* pObject)
{
	m_Objects.push_back(pObject);
}

void GameScene::AddPostProcessingEffect(PostProcessingEffect* pPostProEffect, unsigned int priority)
{
	m_PostProEffects.insert(make_pair(priority, pPostProEffect));
}
