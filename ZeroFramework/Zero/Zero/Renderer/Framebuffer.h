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

		struct Properties
		{
			Properties() :
				Width(1280),
				Height(720),
				mMSAASamples(0),
				mMaxMSAASamplesSupported(0)
			{

			}

			Properties(unsigned width, unsigned height, unsigned msaaSamples) :
				Width(width),
				Height(height),
				mMSAASamples(msaaSamples),
				mMaxMSAASamplesSupported(0)
			{

			}

			unsigned getMaxMSAASamplesSupported() const
			{
				return mMaxMSAASamplesSupported;
			}

			void setMaxMSAASamplesSupported(unsigned maxSamples)
			{
				mMaxMSAASamplesSupported = maxSamples;
				setMSAASamples(mMSAASamples);
			}

			unsigned getMSAASamples() const
			{
				return mMSAASamples;
			}

			void setMSAASamples(unsigned sampleCount)
			{
				mMSAASamples = sampleCount == 0 ? 1 : sampleCount;
				if (mMSAASamples > mMaxMSAASamplesSupported) {
					mMSAASamples = mMaxMSAASamplesSupported;
				}
			}

			unsigned Width;
			unsigned Height;

		private:
			unsigned mMSAASamples;
			unsigned mMaxMSAASamplesSupported;
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
		virtual void blit() const = 0;
		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual unsigned getTextureHandle() const = 0;
		virtual const glm::vec2& getPixelSize() const = 0;
		virtual const Properties& getProperties() const = 0;
		virtual void setSize(unsigned width, unsigned height) = 0;
		virtual void setSize(const glm::vec2& size)
		{
			if (size.x < 0.f || size.y < 0.f) return;

			setSize((unsigned)size.x, (unsigned)size.y);
		}

		/**
		* \brief Sets the MSAA (Multi-sample anti-aliasing) level for this framebuffer.
		*
		* \param msaaLevel The antialiasing level.
		* - 1 - (=1)  single sample
		* - 2 - (=2)  two samples
		* - 4 - (=4)  four samples
		* - 8 - (=8)  eight samples
		* - 16 - (=16) 16 samples
		* - .
		* - .
		* - .
		*/
		virtual unsigned setMSAASamples(unsigned msaaLevel) = 0;

	public:
		static Ref<Framebuffer> Create(const Properties& props = Properties());
		static void BindDefault();
		static unsigned GetMaxSamples();
		static std::pair<unsigned, unsigned> GetMaxViewportSize();
	};
};
