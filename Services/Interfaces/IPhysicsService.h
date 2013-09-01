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
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "../../Helpers/Namespace.h"
#include "NxPhysics.h"
#include "ControllerManager.h"
#include <NxController.h>
#include "NxCapsuleController.h"
class NxShape;
class GameScene;
class GameObject;
class PhysicsAllocator;

#pragma comment(lib, "PhysXLoader.lib")
#pragma comment(lib, "NxCharacter.lib")

#include <map>

class IPhysicsService : public NxUserTriggerReport
{
public:
	IPhysicsService(void){}
	virtual ~IPhysicsService(void){}

	virtual void Initialize(void)=0;

	virtual void Simulate(float gameTime)=0;
	virtual void FetchResults(void)=0;

	virtual void RenderDebugInfo(const tt::GameContext& context)=0;

	virtual NxPhysicsSDK* GetPhysicsSDK(void)=0;

	virtual NxScene* CreateScene(void)=0;
	virtual void RemovePhysicsScene(GameScene* scene)=0;
	
	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)=0;

	virtual NxController* CreateController(GameScene* pGameScene, NxCapsuleControllerDesc desc)=0;

	virtual NxShape* GetClosestShape(NxRay& ray, NxShapesType shapetype, float distance) const=0;

	virtual void SetActiveScene(NxScene* pScene)=0;
	virtual NxScene* GetActiveScene(void) const=0;

	virtual GameObject* Pick(const POINT& mousePosition, const tt::GameContext& context) const=0;

private:

	IPhysicsService(const IPhysicsService& t);// = delete;
	IPhysicsService& operator=(const IPhysicsService& t);// = delete;
};
