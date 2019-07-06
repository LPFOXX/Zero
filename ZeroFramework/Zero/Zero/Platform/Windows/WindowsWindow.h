#pragma once

#include <Zero/Window.h> 

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

	private:
		static bool sGLFWInitialized;

	private:
		struct WindowData
		{
			unsigned Width;
			unsigned Height;
			bool VSync;
			EventCallback callback;
		} mData;

		std::string mTitle;

		GLFWwindow* mWindowHandle;
	};
}
