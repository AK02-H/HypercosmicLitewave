#pragma once
#include <HAPI_lib.h>
#include <unordered_map>
#include <HAPI_InputCodes.h>
//Blackbox

enum class InputMap
{
	eLeft,
	eRight,
	eUp,
	eDown,
	eFire,
	eFireB
};

enum class ControllerType
{
	typeKeyboard,
	typeController
};

class InputHandler
{
private:

	const HAPISPACE::HAPI_TKeyboardData& m_keyboardData;
	const HAPISPACE::HAPI_TControllerData& m_controllerData;

	ControllerType controlType{ ControllerType::typeKeyboard };


	int m_controllerID = 0;

	std::vector<std::pair<std::vector<int>, InputMap>> m_inputKeyboardDefinitions
	{
		{ {HK_LEFT, 'A'}, InputMap::eLeft},

		{ {HK_RIGHT, 'D'}, InputMap::eRight},

		{ {HK_UP, 'W'}, InputMap::eUp },

		{ {HK_DOWN, 'S'}, InputMap::eDown },

		{ {HK_SPACE, 'Z'}, InputMap::eFire },

		{ {HK_LSHIFT, 'X'}, InputMap::eFireB },
	};

	std::vector<std::pair<std::vector<int>, InputMap>> m_inputControllerDefinitions
	{
		{ {HK_DIGITAL_DPAD_LEFT}, InputMap::eLeft},

		{ {HK_DIGITAL_DPAD_RIGHT}, InputMap::eRight},

		{ {HK_DIGITAL_DPAD_UP}, InputMap::eUp },

		{ {HK_DIGITAL_DPAD_DOWN}, InputMap::eDown },

		{ {HK_DIGITAL_A}, InputMap::eFire },

		{ {HK_DIGITAL_B}, InputMap::eFireB },
	};

	std::vector<std::pair<std::vector<int>, InputMap>> m_inputJoystickDefinitions
	{
		{ {HK_DIGITAL_DPAD_LEFT}, InputMap::eLeft},

		{ {HK_DIGITAL_DPAD_RIGHT}, InputMap::eRight},

		{ {HK_DIGITAL_DPAD_UP}, InputMap::eUp },

		{ {HK_DIGITAL_DPAD_DOWN}, InputMap::eDown },

		{ {HK_DIGITAL_A, HK_DIGITAL_Y}, InputMap::eFire },

		{ {HK_DIGITAL_B, HK_DIGITAL_X}, InputMap::eFireB },
	};

	std::unordered_map<InputMap, bool> m_keysDown =
	{
		{InputMap::eLeft, false},
		{InputMap::eRight, false},
		{InputMap::eUp, false},
		{InputMap::eDown, false},
		{InputMap::eFire, false},
		{InputMap::eFireB, false},
	};

	std::unordered_map<InputMap, bool> m_keysHeld =
	{
		{InputMap::eLeft, false},
		{InputMap::eRight, false},
		{InputMap::eUp, false},
		{InputMap::eDown, false},
		{InputMap::eFire, false},
		{InputMap::eFireB, false},
	};

	std::unordered_map<InputMap, bool> m_keysUp =
	{
		{InputMap::eLeft, false},
		{InputMap::eRight, false},
		{InputMap::eUp, false},
		{InputMap::eDown, false},
		{InputMap::eFire, false},
		{InputMap::eFireB, false},
	};


	

	bool isRumbling = false;
	float timer_rumble = 0;
	int currentRumbleStrength_left = 0;
	int currentRumbleStrength_right = 0;

public:

	InputHandler(ControllerType type = ControllerType::typeKeyboard) : m_keyboardData(HAPI.GetKeyboardData()), m_controllerData(HAPI.GetControllerData(m_controllerID))
	{
		controlType = type;
	}

	void Handle(float dt);

	void SetControllerID(int id) { m_controllerID = id; };

	bool GetInputDown(InputMap input)
	{
		return m_keysDown[input];
	}

	bool GetInputHold(InputMap input)
	{
		return m_keysHeld[input];
	}

	bool GetInputUp(InputMap input)
	{
		return m_keysUp[input];
	}

	void ProcessInputData(std::vector<std::pair<std::vector<int>, InputMap>>& inputDefinitions, const HAPISPACE::HAPI_TControllerData& substitute = HAPISPACE::HAPI_TControllerData());
	
	void InvokeRumble(int strength_left, int strength_right, float time);
	
};

