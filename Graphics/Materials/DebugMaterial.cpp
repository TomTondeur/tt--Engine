#include "DebugMaterial.h"

DebugMaterial::DebugMaterial(void):Material(_T("Resources/DebugWireEffect.fx")){}

DebugMaterial::~DebugMaterial(void){}

//Methods

void DebugMaterial::UpdateEffectVariables(const tt::GameContext& context)
{

}