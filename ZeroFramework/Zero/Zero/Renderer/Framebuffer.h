#pragma once

#include "Zero/Core/Core.h"

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

			FramebufferProperties(unsigned width, unsigned height, unsigned mssaLevel) :
				Width(width),
				Height(height),
				MSSALevel(mssaLevel)
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

		static void BindDefault();

		virtual void draw() const = 0;
		virtual void blit() const = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual unsigned getTextureHandle() const = 0;

		virtual const glm::vec2& getPixelSize() const = 0;

		static Ref<Framebuffer> Create(const FramebufferProperties& props = FramebufferProperties());	
	};
};
