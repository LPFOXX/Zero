#pragma once
#include "Events/Event.h"

namespace zr
{
	struct WindowConfig
	{
		WindowConfig(const std::string& title = "ZeRo", unsigned width = 1280, unsigned height = 600) :
			Title(title),
			Width(width),
			Height(height)
		{
			
		}
		
		std::string Title;
		unsigned Width;
		unsigned Height;
	};
	
	class Window
	{
		public:
			using EventCallback = std::function<void(Event&)>;
			
			static Window* Create(const WindowConfig& wc = WindowConfig());
			virtual ~Window() {};
			
			virtual void onUpdate() = 0;
			virtual unsigned getWidth() const = 0;
			virtual unsigned getHeight() const = 0;
			virtual void* getNativeHandle() const = 0;
			
			virtual void setEventCallback(const EventCallback& callback) = 0;
			virtual void setVSync(bool enabled) = 0;
			virtual bool isVSync() = 0;
	};
}
