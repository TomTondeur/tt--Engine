#include "TTscene.h"

#include "SceneObjects/FreeCamera.h"
#include "SceneObjects/Object3D.h"

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
}