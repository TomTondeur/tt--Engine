#include "GameScene.h"
#include "../Components/CameraComponent.h"

GameScene* GameScene::s_pActiveScene = nullptr;

GameScene::GameScene():m_pPhysicsScene(nullptr), m_pActiveCamera(nullptr){}
GameScene::~GameScene()
{
	for(auto pObj : m_Objects)
		delete pObj;
}
	
void GameScene::InitializeScene(void)
{
	for(auto pObj : m_Objects){
		pObj->Initialize();
		pObj->InitializeObject();
	}
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
