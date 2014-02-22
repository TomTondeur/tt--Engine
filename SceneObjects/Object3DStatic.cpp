// Copyright � 2013 Tom Tondeur
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

#include "Object3DStatic.h"
#include "../Services/ServiceLocator.h"
#include "../Graphics/Materials/Object3DMaterial.h"

Object3DStatic::Object3DStatic(const std::tstring& meshFile)
{
	auto pTransform = new TransformComponent();
	auto pModel = new ModelComponent(meshFile,pTransform);
	
	SetComponent<TransformComponent>(pTransform);
	SetComponent<ModelComponent>(pModel);
}

Object3DStatic::~Object3DStatic(void)
{

}

//Methods
extern int tt::g_SceneIndex;
void Object3DStatic::Initialize(void)
{
	auto pMat = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<Object3DMaterial>( _T("BasicMaterial") );	
	GetComponent<ModelComponent>()->SetMaterial(pMat);
}

void Object3DStatic::Draw(const tt::GameContext& context)
{
	if(tt::g_SceneIndex == 3)
		GetComponent<ModelComponent>()->DrawDeferred(context);
	else if(tt::g_SceneIndex > 1)
		GetComponent<ModelComponent>()->Draw(context);
}

void Object3DStatic::GenerateShadows(const tt::GameContext& context)
{
	if(tt::g_SceneIndex == 4)
		GetComponent<ModelComponent>()->GenerateShadows(context);
}
