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
