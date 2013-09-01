#include "DefaultPhysicsService.h"
#include "../ServiceLocator.h"
#include "../../Scenegraph/GameScene.h"
#include "../../Components/Physics/RigidBodyComponent.h"
#include "../../Diagnostics/DebugRenderer.h"
#include "../../Helpers/TemplateUtil.h"
#include "../../AbstractGame.h"
#include "../../Components/CameraComponent.h"

#define INV255f 0.0039215686274509803921568627451f

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

		if (code < NXE_DB_INFO){
			tstringstream ss;
			ss << _T("Physics SDK ERROR") << endl << endl << _T("Message: \n") << StringToTstring(message) << endl;
			MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(ss.str(),LogLevel::Error);
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
		tcout << _T("PhysX SDK says: ") << StringToTstring(message) << endl;
	}

} myOutputStream;

//DefaultPhysicsService

DefaultPhysicsService::DefaultPhysicsService(void):m_pPhysicsSDK(nullptr), m_pControllerManager(nullptr), m_pAllocator(nullptr), m_pDebugRenderer(nullptr), m_pActiveScene(nullptr){}

DefaultPhysicsService::~DefaultPhysicsService(void)
{
	if(m_pControllerManager)
		NxReleaseControllerManager(m_pControllerManager);
	
	if(m_pPhysicsSDK)
		m_pPhysicsSDK->release();
		
	delete m_pAllocator;
	delete m_pDebugRenderer;
}

void DefaultPhysicsService::Initialize(void)
{
	//create PhysicsSDK object
	m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, &myOutputStream);
	if(!m_pPhysicsSDK)
		MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(_T("Wrong PhysX SDK DLL version"), LogLevel::Error);

#ifndef NDEBUG
	m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
	m_pDebugRenderer = new DebugRenderer();
	m_pDebugRenderer->Initialize();
#endif

	m_pAllocator = new PhysicsAllocator();
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
}

void DefaultPhysicsService::Simulate(float gameTime)
{
	gameTime = max(gameTime, .2f); //Cap gametime to .2f to prevent physX scene explosions
	NxReal myTimestep = gameTime;

	m_pActiveScene->simulate(myTimestep);
	m_pActiveScene->flushStream();
}

void DefaultPhysicsService::FetchResults(void)
{
	m_pActiveScene->fetchResults(NX_ALL_FINISHED ,true);
}

void DefaultPhysicsService::RenderDebugInfo(const tt::GameContext& context)
{
	#ifndef NDEBUG
		auto pRenderables = m_pActiveScene->getDebugRenderable();
		auto pCurrLine = pRenderables->getLines();
		unsigned int nrOfLines = static_cast<unsigned int>(pRenderables->getNbLines());
	
		tt::Vector4 color;
		color.w = 1;

		for(unsigned int i = 0; i < nrOfLines; ++i, ++pCurrLine){
			color.x = ( ( pCurrLine->color & 0x00ff0000 ) >> 16 ) * INV255f;
			color.y = ( ( pCurrLine->color & 0x0000ff00 ) >> 8  ) * INV255f;
			color.z =   ( pCurrLine->color & 0x000000ff )		  * INV255f;
			
			m_pDebugRenderer->DrawLine(	static_cast<tt::Vector3>(pCurrLine->p0),
										static_cast<tt::Vector3>(pCurrLine->p1),
										color);
		}

		m_pDebugRenderer->Flush(context);
	#endif
}

NxPhysicsSDK* DefaultPhysicsService::GetPhysicsSDK(void)
{
	return m_pPhysicsSDK;
}

NxScene* DefaultPhysicsService::CreateScene(void)
{
	//create a scene object
	NxSceneDesc sceneDesc;
	sceneDesc.gravity.set(0,-9.81f,0);

	NxScene *pScene = m_pPhysicsSDK->createScene(sceneDesc);
	if(!pScene)
		MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(_T("Scene creation failed"), LogLevel::Error);

	// Set default material
	NxMaterial* defaultMaterial = pScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.5f);
	defaultMaterial->setStaticFriction(0.6f);
	defaultMaterial->setDynamicFriction(0.2f);

	NxReal myTimestep = 1/60.0f;
	pScene->setTiming(myTimestep/4.0f,4,NX_TIMESTEP_FIXED);//4 substeps

	//tell the sdk to use this as the trigger report object
	pScene->setUserTriggerReport(this);

	return pScene;
}

void DefaultPhysicsService::RemovePhysicsScene(GameScene* pScene)
{	
	if (!pScene)
		return;
		
	for(auto pair : m_Controllers)
		if (pair.first == pScene->GetName())
			m_pControllerManager->releaseController(*(pair.second));

	m_Controllers.erase(pScene->GetName());
	m_pPhysicsSDK->releaseScene(*pScene->GetPhysicsScene());
}
	
void DefaultPhysicsService::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
{
	NxActor& triggerActor = triggerShape.getActor();
	NxActor& otherActor = otherShape.getActor();
	if (triggerActor.userData == nullptr || otherActor.userData == nullptr) return;

	auto otherObject   = static_cast<RigidBodyComponent*>(otherActor.userData);
	auto triggerObject = static_cast<RigidBodyComponent*>(triggerActor.userData);
	
	switch(status){
		case NX_TRIGGER_ON_ENTER:
			triggerObject->OnTriggerEnter(otherObject);
			break;
		case NX_TRIGGER_ON_LEAVE:
			triggerObject->OnTriggerLeave(otherObject);
			break;
	}
}

NxController* DefaultPhysicsService::CreateController(GameScene* pGameScene, NxCapsuleControllerDesc desc)
{
	NxController* pController = m_pControllerManager->createController(pGameScene->GetPhysicsScene(), desc);

	m_Controllers.insert(pair<std::tstring, NxController*>(pGameScene->GetName(), pController));

	return pController;
}

NxShape* DefaultPhysicsService::GetClosestShape(NxRay& ray, NxShapesType shapetype, float distance) const
{
	NxRaycastHit hit;
	NxShape* result = m_pActiveScene->raycastClosestShape(ray,shapetype,hit,1,distance);
	return result;
}

void DefaultPhysicsService::SetActiveScene(NxScene* pScene)
{
	m_pActiveScene = pScene;
}

NxScene* DefaultPhysicsService::GetActiveScene(void) const
{
	return m_pActiveScene;
}

GameObject* DefaultPhysicsService::Pick(const POINT& mousePosition, const tt::GameContext& context) const
{
	tt::Vector2 ndcCoords;
	tt::Vector2 halfDimensions(context.vpInfo.width * .5f, context.vpInfo.height * .5f);
	auto pCamera = context.pGame->GetActiveScene()->GetActiveCamera();
	
	//Calculate NDC
	ndcCoords.x = (mousePosition.x - halfDimensions.x)/halfDimensions.x;
	ndcCoords.y = (halfDimensions.y - mousePosition.y)/halfDimensions.y;

	//Transform near and far point
	tt::Matrix4x4 matViewProj = (pCamera->GetView() * pCamera->GetProjection()).Inverse();
	tt::Vector3 start	= tt::Vector3(ndcCoords.x, ndcCoords.y, 0).TransformPoint(matViewProj);
	tt::Vector3 end		= tt::Vector3(ndcCoords.x, ndcCoords.y, 1).TransformPoint(matViewProj);
	tt::Vector3 direction = tt::Vector3::Normalize(end - start);

	//Raycast
	NxRay ray(static_cast<NxVec3>(start), static_cast<NxVec3>(direction) );
	auto pShape = GetClosestShape(ray, NX_ALL_SHAPES, NX_MAX_F32);

	if(!pShape)
		return nullptr;
	
	//Return parent GameObject
	return static_cast<RigidBodyComponent* >(pShape->getActor().userData)->GetParent();
}
