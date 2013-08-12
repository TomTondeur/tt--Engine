#pragma once

#include "../Interfaces/IInputService.h"
#include "../../Timer.h"

struct InputAction{
	InputAction(unsigned char _key, KeyState _targetState);

	unsigned char key;
	KeyState targetState;
	KeyState currentState;
	unsigned short milliSecondsSincePress;
};

class DefaultInputService : public IInputService
{
public:
	DefaultInputService();
	virtual ~DefaultInputService();

	virtual void Update(const tt::GameContext& context) override;
	virtual void AddInputAction(InputActionId action, unsigned char key, KeyState state) override;
	virtual bool IsActionTriggered(InputActionId action) override;
	virtual tt::Vector2 GetMousePosition(void) override;
	virtual tt::Vector2 GetMouseMovement(void) override;

private:
	static const unsigned short sc_MilliSecondsUntilHeld = 1000;
	tt::Timer m_UpdateTimer;
	tt::Vector2 m_OldMousePosition, m_MousePosition, m_MouseMovement;

	std::map<InputActionId, InputAction> m_ActionsToMonitor;

	DefaultInputService(const DefaultInputService& src);// = delete;
	DefaultInputService& operator=(const DefaultInputService& src);// = delete;
};
