#pragma once

namespace zr
{
	enum class EventType
	{
		None = 0,
		WindowClose,			/**< */
		WindowResize,			/**< */
		WindowMove,				/**< */
		WindowGainedFocus,		/**< */
		WindowLostFocus,		/**< */
		
		KeyPressed,				/**< */
		KeyReleased,			/**< */
		KeyTyped,				/**< */
		
		MouseButtonPressed,		/**< */
		MouseButtonReleased,	/**< */
				
		MouseMove,				/**< */
		MouseScroll,			/**< */

		ApplicationEvent,

		RenderWindowResize,
		RenderWindowFramebufferResize
	};
	
	enum EventFamily
	{
		None = 0,
		ApplicationEvent	= 1 << 0,
		InputEvent		 	= 1 << 1,
		KeyboardEvent		= 1 << 2,
		MouseEvent			= 1 << 3,
		MouseButtonEvent	= 1 << 4
	};
	
	class Event
	{
		public:
			Event();
			virtual ~Event();
			
			virtual EventType getType() const = 0;
			virtual const char* getName() const = 0;
			virtual unsigned getFamilyFlags() const = 0;
			virtual std::string toString() const;
			void setHandled(bool handled = true);
			
			inline bool isInFamily(EventFamily family) const
			{
				return (getFamilyFlags() & family);
			}

			inline bool isHandled() const
			{
				return mIsHandled;
			}

			template <typename OStream>
			friend OStream& operator<<(OStream& os, const Event& e)
			{
				return os << e.toString();
			}

			inline std::ostream& operator<<(std::ostream& os)
			{
				return os << toString();
			}
			
		protected:
			bool mIsHandled;
	};
}
