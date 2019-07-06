#include "Event.h"

namespace zr
{
	class ApplicationEvent : public Event
	{
		ApplicationEvent();
		virtual ~ApplicationEvent();
		
		static EventType getStaticType();
		virtual EventType getType() const override;
		virtual const char* getName() const override;
		virtual unsigned getFamilyFlags() const override;
	};
}
