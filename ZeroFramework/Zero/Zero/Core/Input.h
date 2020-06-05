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

		inline static bool IsKeyPressed(Keyboard key)
		{
			return sInstance->isKeyPressedImpl(key);
		}

		inline static bool IsKeyPressed(unsigned key)
		{
			return IsKeyPressed((Keyboard)key);
		}

		inline static bool IsMouseButtonPressed(MouseButton button)
		{
			return sInstance->isMouseButtonPressedImpl(button);
		}

		inline static bool IsMouseButtonPressed(unsigned button)
		{
			return sInstance->IsMouseButtonPressed((MouseButton)button);
		}

		inline static std::pair<float, float> GetMousePosition()
		{
			return sInstance->getMousePositionImpl();
		}

		inline static float GetMouseX()
		{
			return sInstance->getMouseXImpl();
		}

		inline static float GetMouseY()
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

	template <typename KeyType>
	class InputMapper
	{
		enum class InputType
		{
			None = 0,
			MouseButtonPressed,
			MouseButtonRelease,

			KeyPressed,
			KeyReleased
		};

		struct InputKey
		{
			InputKey() :
				mInputType(0),
				mKeyOrButtonCode(0)
			{

			}

			InputKey(const InputType& inputType, unsigned keyOrButtonCode) :
				mInputType(inputType),
				mKeyOrButtonCode(keyOrButtonCode)
			{

			}

			InputType mInputType;
			unsigned mKeyOrButtonCode;
		};

	public:
		InputMapper() :
			mKeyMap()
		{
		}
		virtual ~InputMapper() = default;

		template <typename E>	// Event
		InputMapper<KeyType>& map(const KeyType& key, const E& keyOrButtonCode)
		{
			return *this;
		}

		template <>
		InputMapper<KeyType>& map(const KeyType& key, const Keyboard& keyCode)
		{
			mKeyMap.insert({ key, InputKey{ InputType::KeyPressed, (unsigned)keyCode } });
			return *this;
		}

		template <>
		InputMapper<KeyType>& map(const KeyType& key, const MouseButton& mouseButton)
		{
			mKeyMap.insert({ key, InputKey{ InputType::MouseButtonPressed, (unsigned)mouseButton } });
			return *this;
		}

		InputKey& get(const KeyType& key)
		{
			auto& found = mKeyMap.find(key);
			if (found != mKeyMap.end()) {
				return found->second;
			}
			return InputKey();
		}

		bool isInputPressed(const KeyType& key)
		{
			auto& it = mKeyMap.find(key);
			if (it != mKeyMap.end()) {
				if (it->second.mInputType == InputType::KeyPressed) {
					return Input::IsKeyPressed(it->second.mKeyOrButtonCode);
				}
				else if (it->second.mInputType == InputType::MouseButtonPressed) {
					return Input::IsMouseButtonPressed(it->second.mKeyOrButtonCode);
				}
			}
			return false;
		}

	private:
		std::unordered_map<KeyType, InputKey> mKeyMap;
	};

	using InputMapper8 = InputMapper<uint8_t>;
	using InputMapper16 = InputMapper<uint16_t>;
	using InputMapper32 = InputMapper<uint32_t>;
}
