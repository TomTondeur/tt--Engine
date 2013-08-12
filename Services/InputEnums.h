//Include this file when implementing IInputService

enum class KeyState : unsigned char{
	Idle	= 1 << 0,
	Pressed	= 1 << 1,
	Held	= 1 << 2,
	Released= 1 << 3
};

inline KeyState operator|(KeyState a, KeyState b)
{return static_cast<KeyState>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));}

inline bool operator&(KeyState a, KeyState b)
{return (static_cast<unsigned char>(a) & static_cast<unsigned char>(b)) != 0;}

enum class InputActionId
{
	Jump,
	CameraMoveForward,
	CameraMoveBack,
	CameraMoveLeft,
	CameraMoveRight,
	CameraMoveUp,
	CameraMoveDown,
	CameraUnlockRotation
};
