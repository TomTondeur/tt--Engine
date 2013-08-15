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

#include "TTscene.h"

#include "SceneObjects/FreeCamera.h"
#include "SceneObjects/Object3D.h"
#include "Services/ServiceLocator.h"
#include "Graphics/SpriteFont.h"
#include "Graphics/PostProcessingEffect.h"
#include "Graphics/Materials/PostProcessingMaterial.h"

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
	//AddSceneObject(new Object3D());
	
	MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<SpriteFont>(_T("Resources/AgencyFB_12.fnt"));


	pPostProEffect = new PostProcessingEffect( MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<PostProcessingMaterial>(_T("Resources/Blur.fx")) );
	pPostProEffect->Initialize();
}

void TTscene::Draw(const tt::GameContext& context)
{
	MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<SpriteFont>(_T("Resources/AgencyFB_12.fnt"))->DrawText(_T("Hello,\nWorld!\nSo nice to see\nYOU!"), tt::Vector2(0), tt::Vector4(.0f, .0f, .0f, 1.0f) );
	pPostProEffect->Draw(context, nullptr);
}
