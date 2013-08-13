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

#include "../Helpers/stdafx.h"
#include "NxPhysics.h"
#include "ControllerManager.h"
#include <NxController.h>
#include <NxCapsuleController.h>
class NxShape;
class ObjectComponent;
class GameScene;

#pragma comment(lib, "PhysXLoader.lib")
#pragma comment(lib, "NxCharacter.lib")

#include <map>
using namespace std;

class MyAllocator : public NxUserAllocator
{
public:        
    void* malloc(NxU32 size);
    void* mallocDEBUG(NxU32 size,const char *fileName, int line);    
    void* realloc(void * memory, NxU32 size);    
    void free(void * memory);
};

class PhysicsManager : public NxUserTriggerReport
{
public:
	//Default destructor
	virtual ~PhysicsManager(void);
	
	static PhysicsManager* GetInstance(void);

	//Methods
	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);

	bool Initialize(void);
	
	void Simulate(NxScene* pScene, float gameTime) const;
	void FetchResults(NxScene* pScene) const;

	NxPhysicsSDK* GetPhysicsSDK(void) const;
	
	NxScene* CreateScene(void) const;
	void RemovePhysicsScene(GameScene* pScene);

	NxController* CreateController(GameScene* scene, NxCapsuleControllerDesc desc);
	
	NxShape *GetClosestShape(NxScene* scene, NxRay& ray, NxShapesType shapetype, float distance) const;

private:
	//Default constructor
	PhysicsManager(void);

	//Datamembers
	NxPhysicsSDK* m_pPhysicsSDK;

	NxControllerManager* m_pControllerManager;
	MyAllocator* m_pAllocator;
	multimap<std::tstring, NxController*> m_Controllers;

	static PhysicsManager* m_pInstance;

	//Disabling default copy constructor & assignment operator
	PhysicsManager(const PhysicsManager& src);
	PhysicsManager& operator=(const PhysicsManager& src);
};
