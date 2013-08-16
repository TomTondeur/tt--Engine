#include "BlurMaterial.h"
#include "../../../Services/ServiceLocator.h"
#include "../../../Services/InputEnums.h"

BlurMaterial::BlurMaterial(void):Material(_T("Resources/Blur.fx")), m_BlurOffset(0)
{

}

BlurMaterial::~BlurMaterial(void)
{

}

//Methods

void BlurMaterial::UpdateEffectVariables(const tt::GameContext& context)
{
	if( MyServiceLocator::GetInstance()->GetService<IInputService>()->IsActionTriggered(InputActionId::CameraMoveUp) )
		m_BlurOffset += 0.01f;
	else if( MyServiceLocator::GetInstance()->GetService<IInputService>()->IsActionTriggered(InputActionId::CameraMoveDown) )
		m_BlurOffset -= 0.01f;

	SetVariable(_T("BlurOffset"), m_BlurOffset);
}
