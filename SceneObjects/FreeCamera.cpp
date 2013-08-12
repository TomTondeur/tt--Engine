#include "FreeCamera.h"

#include "../Services/ServiceLocator.h"
#include "../Services/InputEnums.h"

FreeCamera::FreeCamera(void):m_MovementSpeed(0.5f),m_RotationSpeed(static_cast<float>(D3DX_PI / 10))
{
	auto pTransform = new TransformComponent();
	SetComponent<TransformComponent>(pTransform);
	SetComponent<CameraComponent>(new CameraComponent(pTransform));
}

FreeCamera::~FreeCamera(void)
{

}

//Methods

void FreeCamera::Initialize(void)
{	
	auto pInputService = MyServiceLocator::GetInstance()->GetService<IInputService>();
	pInputService->AddInputAction(InputActionId::CameraMoveForward, 'Z', KeyState::Held | KeyState::Pressed);
	pInputService->AddInputAction(InputActionId::CameraMoveBack, 'S', KeyState::Held | KeyState::Pressed);
	pInputService->AddInputAction(InputActionId::CameraMoveLeft, 'Q', KeyState::Held | KeyState::Pressed);
	pInputService->AddInputAction(InputActionId::CameraMoveRight, 'D', KeyState::Held | KeyState::Pressed);
	pInputService->AddInputAction(InputActionId::CameraMoveUp, VK_UP, KeyState::Held | KeyState::Pressed);
	pInputService->AddInputAction(InputActionId::CameraMoveDown, VK_DOWN, KeyState::Held | KeyState::Pressed);
	pInputService->AddInputAction(InputActionId::CameraUnlockRotation, VK_LBUTTON, KeyState::Held | KeyState::Pressed);
	GetComponent<TransformComponent>()->Translate(0, 0, -8);
}

void FreeCamera::Update(const tt::GameContext& context)
{
	auto pInputService = MyServiceLocator::GetInstance()->GetService<IInputService>();
	auto pTransform = GetComponent<TransformComponent>();
	
	if(pInputService->IsActionTriggered(InputActionId::CameraMoveForward))
		pTransform->Translate(pTransform->GetForward() * m_MovementSpeed);
	
	if(pInputService->IsActionTriggered(InputActionId::CameraMoveBack))
		pTransform->Translate(pTransform->GetBackward() * m_MovementSpeed);
	
	if(pInputService->IsActionTriggered(InputActionId::CameraMoveLeft))
		pTransform->Translate(pTransform->GetLeft() * m_MovementSpeed);
	
	if(pInputService->IsActionTriggered(InputActionId::CameraMoveRight))
		pTransform->Translate(pTransform->GetRight() * m_MovementSpeed);

	if(pInputService->IsActionTriggered(InputActionId::CameraMoveUp))
		pTransform->Translate(pTransform->GetUp() * m_MovementSpeed);
	
	if(pInputService->IsActionTriggered(InputActionId::CameraMoveDown))
		pTransform->Translate(pTransform->GetDown() * m_MovementSpeed);

	if(!pInputService->IsActionTriggered(InputActionId::CameraUnlockRotation))
		return;

	auto mouseMoveDir = pInputService->GetMouseMovement();
	auto rotationVec = tt::Vector3(mouseMoveDir.x, mouseMoveDir.y, 0) * m_RotationSpeed * .016f;
	
	D3DXMATRIX yawRotationMat, pitchRotationMat, finalRotMat;
	D3DXVECTOR3 transformedRight;
	D3DXMatrixRotationAxis(&yawRotationMat, &D3DXVECTOR3(0,1,0), rotationVec.x);
	D3DXVec3TransformCoord(&transformedRight,&D3DXVECTOR3(1,0,0),&yawRotationMat);
	D3DXMatrixRotationAxis(&pitchRotationMat, &transformedRight, rotationVec.y);
	finalRotMat = yawRotationMat * pitchRotationMat;

	D3DXQUATERNION finalQuat;
	D3DXQuaternionRotationMatrix(&finalQuat, &finalRotMat);

	pTransform->Rotate( tt::Quaternion(finalQuat) );
}
