#pragma once

namespace zr
{
	class Framebuffer
	{
	public:
		/**
		* @brief
		*
		* Specifying the internal format of the color buffer of the framebuffer can be useful
		* when applying post effects such as HDR (high dynamic range) and bloom.
		*
		*/
		enum InternalFormat
		{
			RGBA,
			RGBA16F,
			RGBA32F
		};

		struct FramebufferProperties
		{
			FramebufferProperties() :
				Width(1280),
				Height(600),
				MSSALevel(0)
			{

			}
			unsigned Width;
			unsigned Height;
			unsigned MSSALevel;
		};

		/**
		* @brief
		*
		* @param width
		* @param height
		* @param msaaLevel Multisampling antialiasing level.
		*
		*/
		Framebuffer();
		virtual ~Framebuffer();

		virtual void draw() const = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		static Framebuffer* Create(const FramebufferProperties& props = FramebufferProperties());	
	};
};
