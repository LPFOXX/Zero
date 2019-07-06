#include "../../Input.h"

namespace zr
{
	class WindowsInput : public Input
	{
	public:
		WindowsInput();
		virtual ~WindowsInput();

	private:
		// Inherited via Input
		virtual bool isKeyPressedImpl(Keyboard key) override;
		virtual bool isMouseButtonPressedImpl(MouseButton button) override;
		virtual std::pair<float, float> getMousePositionImpl() override;
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
	};
}
