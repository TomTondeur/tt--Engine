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
#include "Graphics/Materials/PostProcessing/BlurMaterial.h"

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
	AddSceneObject(new Object3D());
	
	pPostProEffect = new PostProcessingEffect( MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<BlurMaterial>(_T("BlurPostpro")) );
	AddPostProcessingEffect(pPostProEffect, 0);
}

void TTscene::Draw(const tt::GameContext& context)
{

}
