#include "Object3D.h"

#include "../Services/ServiceLocator.h"
#include "../Graphics/Material.h"

Object3D::Object3D(void)
{
	auto pTransform = new TransformComponent();
	auto pModel = new ModelComponent(_T("c:/users/user/desktop/box.bin"),pTransform);
	
	SetComponent<TransformComponent>(pTransform);
	SetComponent<ModelComponent>(pModel);
}

Object3D::~Object3D(void)
{

}

//Methods

void Object3D::Initialize(void)
{
	GetComponent<ModelComponent>()->SetMaterial( MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<Material>( _T("BasicMaterial") ) );
}

void Object3D::Update(const tt::GameContext& context)
{
	//GetComponent<TransformComponent>()->Rotate(D3DX_PI * 0.016f, D3DX_PI * 0.016f, D3DX_PI * 0.016f);
}