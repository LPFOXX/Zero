#include "Event.h"

namespace zr
{
	class ApplicationEvent : public Event
	{
	public:
		ApplicationEvent();
		virtual ~ApplicationEvent();

		static EventType GetStaticType();
		virtual EventType getType() const override;
		virtual const char* getName() const override;
		virtual unsigned getFamilyFlags() const override;
	};
}
