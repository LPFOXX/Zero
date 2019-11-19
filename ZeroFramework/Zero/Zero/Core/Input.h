#pragma once

#include "InputCodes.h"

namespace zr
{
	class Input
	{
	public:
		Input()
		{

		}
	
		virtual ~Input()
		{

		}

		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool isKeyPressed(Keyboard key)
		{
			return sInstance->isKeyPressedImpl(key);
		}

		inline static bool isMouseButtonPressed(MouseButton button)
		{
			return sInstance->isMouseButtonPressedImpl(button);
		}

		inline static std::pair<float, float> getMousePosition()
		{
			return sInstance->getMousePositionImpl();
		}

		inline static float getMouseX()
		{
			return sInstance->getMouseXImpl();
		}

		inline static float getMouseY()
		{
			return sInstance->getMouseYImpl();
		}

	protected:
		virtual bool isKeyPressedImpl(Keyboard key) = 0;
		virtual bool isMouseButtonPressedImpl(MouseButton button) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;

	private:
		static Scope<Input> sInstance;
	};
}
