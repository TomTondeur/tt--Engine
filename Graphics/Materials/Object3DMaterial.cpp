#include "Object3DMaterial.h"
#include "../../Services/ServiceLocator.h"

Object3DMaterial::Object3DMaterial(void):Material(_T("Resources/PosNormTex.fx"))
{

}

Object3DMaterial::~Object3DMaterial(void)
{

}

//Methods

void Object3DMaterial::UpdateEffectVariables(const tt::GameContext& context)
{
	SetVariable(_T("DIRECTION"), tt::Vector3(0,-1,0));
	SetVariable(_T("TEXTURE_DIFFUSE"), MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ID3D10ShaderResourceView>(_T("Resources/Vampire_Diffuse.dds")).get());
}