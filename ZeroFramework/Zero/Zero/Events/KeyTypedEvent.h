#include "KeyEvent.h"

namespace zr
{
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode);
		virtual ~KeyTypedEvent();

		// Inherited via Event
		static EventType GetStaticType();
		virtual EventType getType() const override;
		virtual const char* getName() const override;
		virtual unsigned getFamilyFlags() const override;
		virtual std::string toString() const override;
	};
}
