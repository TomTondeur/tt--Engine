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

#include "FreeCamera.h"

#include "../Services/ServiceLocator.h"
#include "../Services/InputEnums.h"

FreeCamera::FreeCamera(void):m_MovementSpeed(500),m_RotationSpeed(static_cast<float>(D3DX_PI / 10)), m_Yaw(0), m_Pitch(0)
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
		pTransform->Translate(pTransform->GetForward() * m_MovementSpeed * context.GameTimer.GetElapsedSeconds(), true);
	
	if(pInputService->IsActionTriggered(InputActionId::CameraMoveBack))
		pTransform->Translate(pTransform->GetBackward() * m_MovementSpeed * context.GameTimer.GetElapsedSeconds(), true);
	
	if(pInputService->IsActionTriggered(InputActionId::CameraMoveLeft))
		pTransform->Translate(pTransform->GetLeft() * m_MovementSpeed * context.GameTimer.GetElapsedSeconds(), true);
	
	if(pInputService->IsActionTriggered(InputActionId::CameraMoveRight))
		pTransform->Translate(pTransform->GetRight() * m_MovementSpeed * context.GameTimer.GetElapsedSeconds(), true);

	if(pInputService->IsActionTriggered(InputActionId::CameraMoveUp))
		pTransform->Translate(pTransform->GetUp() * m_MovementSpeed * context.GameTimer.GetElapsedSeconds(), true);
	
	if(pInputService->IsActionTriggered(InputActionId::CameraMoveDown))
		pTransform->Translate(pTransform->GetDown() * m_MovementSpeed * context.GameTimer.GetElapsedSeconds(), true);

	if(!pInputService->IsActionTriggered(InputActionId::CameraUnlockRotation))
		return;

	auto mouseMovement = pInputService->GetMouseMovement();
	mouseMovement *= m_RotationSpeed * context.GameTimer.GetElapsedSeconds();
	m_Yaw += mouseMovement.x;
	m_Pitch += mouseMovement.y;

	tt::Quaternion yawQuat(tt::Vector3::j, m_Yaw);
	tt::Vector3 rotatedRight = tt::Vector3::i.TransformPoint(yawQuat);
	tt::Quaternion pitchQuat(rotatedRight, m_Pitch);

	pTransform->Rotate(yawQuat * pitchQuat);
}
