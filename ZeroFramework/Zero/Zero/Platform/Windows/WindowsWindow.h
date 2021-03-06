#pragma once

#include <Zero/Core/Core.h>
#include <Zero/Core/Window.h>
#include <Zero/Renderer/GraphicsContext.h>

namespace zr
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowConfig& wc);
		virtual ~WindowsWindow();

		// Inherited via Window
		virtual void onUpdate() override;
		virtual unsigned getWidth() const override;
		virtual unsigned getHeight() const override;
		virtual void* getNativeHandle() const override;

		virtual void setEventCallback(const EventCallback& callback) override;
		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() override;

		virtual void setIcon(const std::string& filePath) const;
		virtual void setIcon(const Image& image) const;

		virtual void captureMouseCursor(bool capture = true) override;

	private:
		static unsigned sGLFWWindowCount;

	private:
		struct WindowData
		{
			WindowData() :
				Width(0),
				Height(0),
				VSync(true),
				callback()
			{

			}

			unsigned Width;
			unsigned Height;
			bool VSync;
			EventCallback callback;
		} mData;

		std::string mTitle;

		GLFWwindow* mWindowHandle;
		Scope<GraphicsContext> mContext;
	};
}
