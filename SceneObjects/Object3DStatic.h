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

#pragma once

#include "../Scenegraph/SceneObject.h"

#include "../Components/TransformComponent.h"
#include "../Components/ModelComponent.h"
#include "../Components/Physics/RigidBodyComponent.h"
#include "../Components/Physics/Colliders/MeshColliderComponent.h"
#include "../Components/ScriptComponent.h"

class Object3DStatic : public GenericSceneObject<TransformComponent, ModelComponent>
{
public:
	//Default constructor & destructor
	Object3DStatic(const std::tstring& meshFile);
	virtual ~Object3DStatic(void);
	
	//Methods
	virtual void Initialize(void) override;
	virtual void Draw(const tt::GameContext& context) override;
	
	virtual void GenerateShadows(const tt::GameContext& context) override;

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	Object3DStatic(const Object3DStatic& src);
	Object3DStatic& operator=(const Object3DStatic& src);
};
