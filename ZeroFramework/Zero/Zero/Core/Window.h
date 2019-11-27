#pragma once

#include "../Core/Core.h"
#include "Events/Event.h"
#include "../Image.h"

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
			
			static Scope<Window> Create(const WindowConfig& wc = WindowConfig());
			virtual ~Window() {};
			
			virtual void onUpdate() = 0;
			virtual unsigned getWidth() const = 0;
			virtual unsigned getHeight() const = 0;
			virtual void* getNativeHandle() const = 0;
			
			virtual void setEventCallback(const EventCallback& callback) = 0;
			virtual void setVSync(bool enabled) = 0;
			virtual bool isVSync() = 0;

			virtual void setIcon(const std::string& filePath) const = 0;
			virtual void setIcon(const Image& image) const = 0;

			virtual void captureMouseCursor(bool capture = true) = 0;
	};
}
