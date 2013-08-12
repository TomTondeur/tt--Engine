#include "DefaultInputService.h"

#include "../ServiceLocator.h"
#include "../../Graphics/Window.h"
#include "../InputEnums.h"

using namespace std;

InputAction::InputAction(unsigned char _key, KeyState _targetState):key(_key),targetState(_targetState),currentState(KeyState::Idle),milliSecondsSincePress(0){}

DefaultInputService::DefaultInputService()
{
	m_UpdateTimer.Start();
}

DefaultInputService::~DefaultInputService(){}

void DefaultInputService::Update(const tt::GameContext& context)
{
	m_UpdateTimer.Tick();
	int milliSecondsSinceLastCheck = (int)(m_UpdateTimer.GetElapsedSeconds() * 1000);

	//Virtual Key monitoring
	for(auto& action : m_ActionsToMonitor)
	{
		short keyState = GetAsyncKeyState(action.second.key);
		if(keyState < 0) //Key down
			switch(action.second.currentState){
			case KeyState::Idle:
				action.second.currentState = KeyState::Pressed;
				break;
			case KeyState::Pressed:
				action.second.milliSecondsSincePress += milliSecondsSinceLastCheck;
				if(action.second.milliSecondsSincePress > sc_MilliSecondsUntilHeld){
					action.second.currentState = KeyState::Held;
					action.second.milliSecondsSincePress = 0;
				}
				break;
			}
		else //Key up
			switch(action.second.currentState){
			case KeyState::Released:
				action.second.currentState = KeyState::Idle;
				break;
			case KeyState::Pressed:
			case KeyState::Held:
				action.second.currentState = KeyState::Released;
				break;
			}
	}

	//Mouse tracking
	auto pWindow = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetWindow();
	POINT mouseCoords;

	m_OldMousePosition = m_MousePosition;

	if(!GetCursorPos(&mouseCoords))
		MyServiceLocator::GetInstance()->GetService<DebugService>()->LogWin32Error(GetLastError(), __LINE__, __FILE__);
	
	ScreenToClient(pWindow->GetHandle(),&mouseCoords);

	m_MousePosition.x = static_cast<float>(mouseCoords.x);
	m_MousePosition.y = static_cast<float>(mouseCoords.y);
}
	
void DefaultInputService::AddInputAction(InputActionId action, unsigned char key, KeyState state)
{
	m_ActionsToMonitor.insert(make_pair(action, InputAction(key, state)));
}

bool DefaultInputService::IsActionTriggered(InputActionId action)
{
	if(GetForegroundWindow() != MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetWindow()->GetHandle())
		return false;

	auto it = m_ActionsToMonitor.find(action);
	return ( it != m_ActionsToMonitor.end() ) && (it->second.targetState & it->second.currentState);
}

tt::Vector2 DefaultInputService::GetMousePosition(void)
{	
	return m_MousePosition;
}

tt::Vector2 DefaultInputService::GetMouseMovement(void)
{
	return m_MousePosition - m_OldMousePosition;
}
