#include "InputHandler.h"

void InputHandler::Handle(float dt = 0) //dt is mostly not necessary here, only ever used for rumble timer
{
	
	switch (controlType)
	{
	case ControllerType::typeKeyboard:	
	{
		ProcessInputData(m_inputKeyboardDefinitions);
		break;
	}
		
	case ControllerType::typeController:
	{

		const HAPISPACE::HAPI_TControllerData& controller = HAPI.GetControllerData(m_controllerID);	//??
		
		if (controller.isAttached)
		{
			ProcessInputData(m_inputControllerDefinitions, controller);

			if (isRumbling)
			{
				HAPI.SetControllerRumble(m_controllerID, currentRumbleStrength_left, currentRumbleStrength_right);
				timer_rumble -= dt;

				if (timer_rumble <= 0)
				{
					isRumbling = false;
					HAPI.SetControllerRumble(m_controllerID, 0, 0);

				}
			}
		}


		
		
		break;
	}
	default:
		break;
	}
}

void InputHandler::ProcessInputData(std::vector<std::pair<std::vector<int>, InputMap>> &inputDefinitions, const HAPISPACE::HAPI_TControllerData& substitute)
{
	for (auto& inputMap : inputDefinitions)
	{
		bool isInputted = 0;

		for (auto& key : inputMap.first)
		{
			if (controlType == ControllerType::typeKeyboard)
			{
				isInputted = isInputted || m_keyboardData.scanCode[key];
			}
			else if (controlType == ControllerType::typeController)
			{
				isInputted = isInputted || substitute.digitalButtons[key];

				//Check joysticks, hardcoded input reading for simplicity
				if (inputMap.second == InputMap::eLeft)
				{
					isInputted = isInputted || (substitute.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE);
				}
				else if (inputMap.second == InputMap::eRight)
				{
					isInputted = isInputted || (substitute.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > HK_GAMEPAD_LEFT_THUMB_DEADZONE);
				}
				else if (inputMap.second == InputMap::eUp)
				{
					isInputted = isInputted || (substitute.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > HK_GAMEPAD_LEFT_THUMB_DEADZONE);
				}
				else if (inputMap.second == InputMap::eDown)
				{
					isInputted = isInputted || (substitute.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE);
				}

				//std::cout << m_controllerData.digitalButtons[key] << std::endl;

				
			}
		}

		//Check for the single frames where the key is pressed down and up
		//if there is an input this frame
		if (isInputted)
		{
			//if this input did not exist last frame
			if (!m_keysHeld[inputMap.second])
			{
				m_keysDown[inputMap.second] = true;
			}
			else //but if this input WAS held down last frame
			{
				m_keysDown[inputMap.second] = false;
			}

			//if there was an input this frame then the key is not being released
			m_keysUp[inputMap.second] = false;
		}
		else //if there was no input this frame
		{
			//if there WAS however an input in the previous frame
			if (m_keysHeld[inputMap.second])
			{
				m_keysUp[inputMap.second] = true;
			}
			else //otherwise, if there was also no input in the last frame
			{
				m_keysUp[inputMap.second] = false;
			}

			//if there was no input this frame then the key is definitely not being pressed down
			m_keysDown[inputMap.second] = false;

		}

		//set hold value to whether there is an input or not
		m_keysHeld[inputMap.second] = isInputted;

	}


	//Exclusive joystick checks
	if (controlType == ControllerType::typeController)
	{

	}
}

void InputHandler::InvokeRumble(int strength_left, int strength_right, float time)
{
	currentRumbleStrength_left = strength_left;
	currentRumbleStrength_right = strength_right;
	timer_rumble = time;
	isRumbling = true;
}
