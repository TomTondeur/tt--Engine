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

#include "PhysicsManager.h"
#include "../Components/Physics/RigidBodyComponent.h"
#include "../Scenegraph/GameScene.h"
#include "../Services/ServiceLocator.h"

//MyAllocator Implementation
        
void* MyAllocator::malloc(NxU32 size)
{
    return ::malloc(size);
}
    
void* MyAllocator::mallocDEBUG(NxU32 size,const char *fileName, int line)
{
    return ::_malloc_dbg(size,_NORMAL_BLOCK, fileName, line);
}
    
void* MyAllocator::realloc(void * memory, NxU32 size)
{
    return ::realloc(memory,size);
}
    
void MyAllocator::free(void * memory)
{
	::free(memory);
}

//Error Reporting
//The Physics SDK provides the possibility to supply a user defined stream 
// class for error logging. Basically, the user needs to subclass the 
// abstract base class NxUserOutputStream:
class MyOutputStream : public NxUserOutputStream
{
	void reportError (NxErrorCode code, const char *message, const char* file, int line)
	{
		//this should be routed to the application
		//specific error handling. If this gets hit
		//then you are in most cases using the SDK
		//wrong and you need to debug your code!
		//however, code may  just be a warning or
		//information.

		if (code < NXE_DB_INFO)
		{
			tstringstream ss;
			ss<<_T("Physics SDK ERROR\n\n");
			ss<<_T("Message: \n");
			ss<<message;
			ss<<"\n";
			MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(ss.str(),LogLevel::Error);
			//cout << "SDK Error" << message << endl;
			//exit(1);
		}
	}

	NxAssertResponse reportAssertViolation (const char *message, const char *file,int line)
	{
		//this should not get hit by
		// a properly debugged SDK!
		assert(0);
		return NX_AR_CONTINUE;
	}

	void print (const char *message)
	{
		printf("PhysX SDK says: %s\n", message);
	}

} myOutputStream;

//PhysicsManager Implementation

PhysicsManager* PhysicsManager::m_pInstance = nullptr;

PhysicsManager::PhysicsManager(void):m_pPhysicsSDK(nullptr)
{

}

PhysicsManager::~PhysicsManager(void)
{
	NxReleaseControllerManager(m_pControllerManager);
	if (m_pAllocator) delete m_pAllocator;

	if(m_pPhysicsSDK)m_pPhysicsSDK->release();
}

//Methods

bool PhysicsManager::Initialize(void)
{
	//create PhysicsSDK object
	m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, &myOutputStream);
	if(!m_pPhysicsSDK)
	{
		MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(_T("Wrong PhysX SDK DLL version"), LogLevel::Error,__LINE__,__FILE__);
		exit(-1);
	}
#ifndef NDEBUG
	m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
#endif

	m_pAllocator = new MyAllocator();
	m_pControllerManager = NxCreateControllerManager(m_pAllocator);

	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZATION_SCALE, 1.0f);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_COLLISION_SHAPES, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_ACTOR_AXES, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_BODY_AXES , 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_BODY_JOINT_GROUPS, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_BODY_MASS_AXES, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_COLLISION_AABBS, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_COLLISION_AXES, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_COLLISION_CCD , 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_COLLISION_COMPOUNDS, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_COLLISION_DYNAMIC, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_COLLISION_EDGES, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_COLLISION_SHAPES, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_COLLISION_SPHERES, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_COLLISION_STATIC, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_FORCE_FIELDS, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_JOINT_WORLD_AXES, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_JOINT_LOCAL_AXES, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_JOINT_LIMITS, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_WORLD_AXES, 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_BODY_LIN_VELOCITY , 1);
	m_pPhysicsSDK->setParameter(NxParameter::NX_VISUALIZE_BODY_ANG_VELOCITY  , 1);

	return true;
}

NxScene* PhysicsManager::CreateScene() const
{
	//create a scene object
	NxSceneDesc sceneDesc;
	sceneDesc.gravity.set(0,-9.81f,0);
	NxScene* pScene = m_pPhysicsSDK->createScene(sceneDesc);
	ASSERT(pScene!=nullptr,_T("Failed to create PhysX scene"));

	// Set default physics material
	NxMaterial* pDefaultMaterial = pScene->getMaterialFromIndex(0);
	pDefaultMaterial->setRestitution(0.5f);
	pDefaultMaterial->setStaticFriction(0.6f);
	pDefaultMaterial->setDynamicFriction(0.2f);

	NxReal myTimestep = 1/60.0f;
	pScene->setTiming(myTimestep/4.0f,4,NX_TIMESTEP_FIXED);//4 substeps

	//tell the sdk to use this as the trigger report object
	pScene->setUserTriggerReport(const_cast<PhysicsManager*>(this));

	return pScene;
}

void PhysicsManager::RemovePhysicsScene(GameScene* scene)
{
	if (scene){
		for(auto& pair : m_Controllers)
			if (pair.first == scene->GetName())
				m_pControllerManager->releaseController(*(pair.second));

		m_Controllers.erase(scene->GetName());
		m_pPhysicsSDK->releaseScene(*scene->GetPhysicsScene());
		scene = nullptr;
	}
}

void PhysicsManager::Simulate(NxScene* pScene, float gameTime) const
{
	//prevent physX scene explosions
	if(gameTime>.2f)
		gameTime=.2f;

	NxReal myTimestep = gameTime; //1/60.0f;
	
	pScene->simulate(myTimestep); //Thread starts calculating physics
	
	//Flushes any buffered commands so that they get executed. 
	//Ensures that commands buffered in the system will continue to make forward progress until completion
	pScene->flushStream();
}
void PhysicsManager::FetchResults(NxScene* pScene) const
{
	//Wait until PhysX calculations are done
	pScene->fetchResults(NX_ALL_FINISHED ,true);
}

PhysicsManager* PhysicsManager::GetInstance(void)
{
	if (m_pInstance == nullptr)
		m_pInstance = new PhysicsManager();

	return m_pInstance;
}

void PhysicsManager::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
{
	NxActor& pTriggerActor = triggerShape.getActor();
	NxActor& pOtherActor = otherShape.getActor();
	if (pTriggerActor.userData == nullptr || pOtherActor.userData == nullptr) 
		return;
	
	RigidBodyComponent* pOtherObject = static_cast<RigidBodyComponent*>(pOtherActor.userData);
	RigidBodyComponent* pTriggerObject = static_cast<RigidBodyComponent*>(pTriggerActor.userData);
	
	if(status & NX_TRIGGER_ON_ENTER)
	{	
		pTriggerObject->OnTriggerEnter(pOtherObject);
	}
	else if(status & NX_TRIGGER_ON_LEAVE)
	{
		pTriggerObject->OnTriggerLeave(pOtherObject);
	}
}

NxPhysicsSDK* PhysicsManager::GetPhysicsSDK() const 
{
	return m_pPhysicsSDK;
}

NxController * PhysicsManager::CreateController(GameScene* scene, NxCapsuleControllerDesc desc)
{
	NxController* pController = m_pControllerManager->createController(scene->GetPhysicsScene(), desc);

	m_Controllers.insert(pair<std::tstring, NxController*>(scene->GetName(), pController));

	return pController;
}

NxShape *PhysicsManager::GetClosestShape(NxScene* scene, NxRay& ray, NxShapesType shapetype = NX_STATIC_SHAPES, float distance=100) const{
	NxRaycastHit hit;
	NxShape* result = scene->raycastClosestShape(ray,shapetype,hit,1,distance);
	return result;
}
