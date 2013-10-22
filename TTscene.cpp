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
#include "SceneObjects/ParticleSystem.h"
#include "SceneObjects/Skybox.h"
#include "SceneObjects/Terrain.h"
#include "Services/ServiceLocator.h"
#include "Graphics/SpriteFont.h"
#include "Graphics/PostProcessingEffect.h"
#include "Graphics/Materials/PostProcessing/BlurMaterial.h"
#include "Graphics/GraphicsDevice.h"
#include "Services/InputEnums.h"

TTscene::TTscene(void)
{

}

TTscene::~TTscene(void)
{

}

//Methods

void TTscene::Initialize(void)
{
	auto pCam = new FreeCamera();
	AddSceneObject(pCam);
	SetActiveCamera(pCam->GetComponent<CameraComponent>());
	AddSceneObject(new Object3D(_T("Resources/goblin.ttmesh")));
	//AddSceneObject(new Object3D(_T("Resources/skewedBoxHierarchy.bin")));
	AddSceneObject(new Skybox());
	
	//AddSceneObject(new ParticleSystem() );
	//AddSceneObject( new Terrain( tt::Vector3(10,3,10), 6, _T("Resources/Textures/HeightMap_128x128x16.raw") ) );

	MyServiceLocator::GetInstance()->GetService<IInputService>()->AddInputAction(InputActionId::ToggleVSync, 'V', KeyState::Pressed);
}

void TTscene::Update(const tt::GameContext& context)
{
	if(MyServiceLocator::GetInstance()->GetService<IInputService>()->IsActionTriggered(InputActionId::ToggleVSync)){
		tcout << _T("Toggle") << endl;
		MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->ToggleVSync();
	}
}

void TTscene::Draw(const tt::GameContext& context)
{

}
