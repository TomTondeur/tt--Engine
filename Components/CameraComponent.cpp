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

#include "CameraComponent.h"
#include "../Scenegraph/GameScene.h"
#include "TransformComponent.h"

using namespace tt;

CameraComponent::CameraComponent(TransformComponent* pParentTransform):m_MatView(Matrix4x4::Identity)
																	,m_MatProj(Matrix4x4::Identity)
																	,m_MatViewInv(Matrix4x4::Identity)
																	,m_pParentTransform(pParentTransform)
{
	m_Attributes.fov = static_cast<float>(D3DX_PI)*.33f;
	m_Attributes.orthoSize = 25.0f;
	m_Attributes.usePerspective = true;
	m_Attributes.zNear = 20.0f;
	m_Attributes.zFar = 2500.0f;
}

CameraComponent::~CameraComponent(void)
{

}

//Methods
extern bool g_bAutoCam;
void CameraComponent::Update(const tt::GameContext& context)
{
	float aspect = context.vpInfo.width / (float)context.vpInfo.height;

	if(tt::g_SceneIndex == 3)aspect/=3;

	D3DXMATRIX matProj;

	if(m_Attributes.usePerspective)
		D3DXMatrixPerspectiveFovLH(&matProj, m_Attributes.fov, aspect, m_Attributes.zNear, m_Attributes.zFar);
	else
		D3DXMatrixOrthoLH(&matProj
						, (m_Attributes.orthoSize>0)?m_Attributes.orthoSize*aspect:context.vpInfo.width
						, (m_Attributes.orthoSize>0)?m_Attributes.orthoSize:context.vpInfo.height
						, m_Attributes.zNear
						, m_Attributes.zFar);

	m_MatProj = static_cast<Matrix4x4>(matProj);
	
	D3DXMATRIX outMat;
	
	if(g_bAutoCam)
	{
		auto totalTime = context.GameTimer.GetTotalSeconds();
		D3DXMatrixLookAtLH(&outMat, &D3DXVECTOR3(sinf(totalTime)*200, 100, cosf(totalTime)*200), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
	}else
	{
		D3DXVECTOR3 vEyePt = m_pParentTransform->GetWorldPosition();
		D3DXQUATERNION rotQuat = m_pParentTransform->GetWorldRotation();
		D3DXVECTOR3 vLookat, vUpVec;
		D3DXMATRIX rotTransform;//, outMat;

		D3DXMatrixRotationQuaternion(&rotTransform,&rotQuat);
		D3DXVec3TransformCoord(&vLookat, &D3DXVECTOR3(0,0,1), &rotTransform);
		D3DXVec3TransformCoord(&vUpVec, &D3DXVECTOR3(0,1,0), &rotTransform);
		D3DXMatrixLookAtLH(&outMat, &vEyePt, &(vEyePt+vLookat), &vUpVec);
	}
	m_MatView = tt::Matrix4x4(outMat);

	D3DXMatrixInverse(&outMat,NULL,&outMat);
	m_MatViewInv = tt::Matrix4x4(outMat);
}

void CameraComponent::SetActive(bool b)
{
	ObjectComponent::SetActive(b);
	if(b)
		GameScene::GetActiveScene()->SetActiveCamera(this);
}

const Matrix4x4& CameraComponent::GetView() const
{
	return m_MatView;
}

const Matrix4x4& CameraComponent::GetProjection() const
{
	return m_MatProj;
}

const Matrix4x4& CameraComponent::GetViewInverse() const
{
	return m_MatViewInv;
}

CameraAttributes& CameraComponent::GetAttributes(void)
{
	return m_Attributes;
}
