#pragma once
#include <utility> 

class Input
{
	public:
		static bool isKeyPressed(int keycode) { return inputInstance->isKeyPressedImpl(keycode); }
		static bool isKeyHeld(int keycode) { return inputInstance->isKeyHeldImpl(keycode); }

		static bool isMouseButtonPressed(int button) { return inputInstance->isMouseButtonPressedImpl(button); }
		static std::pair<float,float> getMousePosition() { return inputInstance->getMousePositionImpl(); }
		static bool getMouseX() { return inputInstance->getMouseXImpl(); }
		static bool getMouseY() { return inputInstance->getMouseYImpl(); }
	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;
		virtual bool isKeyHeldImpl(int keycode) = 0;

		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;
	private:
		static Input* inputInstance;
};