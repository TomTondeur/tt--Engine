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

//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// TTscene.cpp
//

#include "TTscene.h"

#include "SceneObjects/FreeCamera.h"
#include "SceneObjects/Object3D.h"
#include "SceneObjects/Object3DStatic.h"
#include "SceneObjects/ParticleSystem.h"
#include "SceneObjects/Skybox.h"
#include "SceneObjects/Terrain.h"
#include "Services/ServiceLocator.h"
#include "Graphics/SpriteFont.h"
#include "Graphics/PostProcessingEffect.h"
#include "Graphics/Materials/PostProcessing/BlurMaterial.h"
#include "Graphics/GraphicsDevice.h"
#include "Services/InputEnums.h"

static ScriptComponent* g_pScript = nullptr;
bool g_bAutoCam = false;

void SetDemoScene(int iScene)
{
	tt::g_SceneIndex = iScene;
}

void SetAutoCamera(bool bAutoCamera)
{
	g_bAutoCam = bAutoCamera;
}

static void InitializeLuaScript()
{
	LuaLink::LuaFunction::Register(SetDemoScene, "SetDemoScene");
	LuaLink::LuaFunction::Register(SetAutoCamera, "SetAutoCamera");
}

TTscene::TTscene(void)
{
	try{
	g_pScript = new ScriptComponent(_T("Resources/Scripts/TestScript.lua"));
	g_pScript->Load(InitializeLuaScript);
	}
	catch(LuaLoadException& e)
	{
		OutputDebugStringA(e.what());
	}
}

TTscene::~TTscene(void)
{
	delete g_pScript;
}

//Methods

void TTscene::Initialize(void)
{
	m_pCam = new FreeCamera();
	AddSceneObject(m_pCam);
	SetActiveCamera(m_pCam->GetComponent<CameraComponent>());
	AddSceneObject(new Object3D(_T("Resources/goblin.ttmesh")));
	AddSceneObject(new Object3DStatic(_T("Resources/Models/environment.ttmesh")));
	AddSceneObject(new Skybox());
	
	auto pBlurMat = new BlurMaterial();
	pBlurMat->LoadEffect();
	AddPostProcessingEffect(new PostProcessingEffect(pBlurMat), 0);
	
	//AddSceneObject(new ParticleSystem() );
	AddSceneObject( new Terrain( tt::Vector3(100,30,100), 6, _T("Resources/Textures/HeightMap_128x128x16.raw") ) );

	MyServiceLocator::GetInstance()->GetService<IInputService>()->AddInputAction(InputActionId::ToggleVSync, 'V', KeyState::Pressed);

	g_pScript->Initialize();
}

void TTscene::Update(const tt::GameContext& context)
{
	if(MyServiceLocator::GetInstance()->GetService<IInputService>()->IsActionTriggered(InputActionId::ToggleVSync)){
		tcout << _T("Toggle") << endl;
		MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->ToggleVSync();
	}

	D3DXMATRIX tempView, tempProj;	
	D3DXMatrixLookAtLH(&tempView, &D3DXVECTOR3(100,200,100), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,0,1));
	D3DXMatrixOrthoLH(&tempProj, 500, 500, 10, 1000);
	auto lightView = tt::Matrix4x4(tempView);
	auto lightProj = tt::Matrix4x4(tempProj);

	Material::SetDominantDirectionalLightViewProjection(lightView * lightProj);
	
	delete g_pScript;
	g_pScript = new ScriptComponent(_T("Resources/Scripts/TestScript.lua"));
	g_pScript->Load(InitializeLuaScript);
	g_pScript->Initialize();
	g_pScript->Update(context);
}

void TTscene::Draw(const tt::GameContext& context)
{

}
