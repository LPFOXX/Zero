#pragma once

#include <Zero/Renderer/Framebuffer.h>
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"

namespace zr
{
	class OpenGLFramebuffer : public Framebuffer
	{
		///**
		//* @brief
		//*
		//* Specifying the internal format of the color buffer of the framebuffer can be useful
		//* when applying post effects such as HDR (high dynamic range) and bloom.
		//*
		//*/
		//enum InternalFormat
		//{
		//	RGBA = GL_RGBA,
		//	RGBA16F = GL_RGBA16F,
		//	RGBA32F = GL_RGBA32F
		//};

	public:
		/**
		* @brief
		*
		* @param width
		* @param height
		* @param msaaLevel Multisampling antialiasing level.
		*
		*/
		OpenGLFramebuffer(const FramebufferProperties& props);
		virtual ~OpenGLFramebuffer();

		virtual void draw() const override;

	public:
		static void BindDefault();

		// Inherited via Framebuffer
		virtual void bind() override;
		virtual void unbind() override;

	private:
		class MultisampledFramebuffer
		{
		public:
			MultisampledFramebuffer(FramebufferProperties props) :
				mFBO(0U),
				mRBO(0U),
				mColorBuffer(0U)
			{
				// Create and bind the framebuffer object.
				GL_ERR_CHECK(glGenFramebuffers(1, &mFBO));
				GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));

				// Create and bind the color buffer object (disguised as a texture object)
				// When MSAA is activated create multisampled offscreen buffers
				// Reserve memory for the color buffer
				GL_ERR_CHECK(glGenTextures(1, &mColorBuffer));
				GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mColorBuffer));
				GL_ERR_CHECK(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, props.MSSALevel, GL_RGBA, props.Width, props.Height, GL_TRUE));;

				// Attach the color buffer object to the framebuffer object
				GL_ERR_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mColorBuffer, 0));

				// Generate, bind and reserve memory for the render buffer
				GL_ERR_CHECK(glGenRenderbuffers(1, &mRBO));
				GL_ERR_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, mRBO));
				GL_ERR_CHECK(glRenderbufferStorageMultisample(GL_RENDERBUFFER, props.MSSALevel, GL_DEPTH24_STENCIL8, props.Width, props.Height));

				// Attach the render buffer
				GL_ERR_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO));

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
					throw std::runtime_error("Unable to create framebuffer.");
				}
			}

			virtual ~MultisampledFramebuffer()
			{
				if (mColorBuffer != 0U) {
					GL_ERR_CHECK(glDeleteTextures(1, &mColorBuffer));
					mColorBuffer = 0U;
				}

				if (mRBO != 0) {
					GL_ERR_CHECK(glDeleteRenderbuffers(1, &mRBO));
					mRBO = 0;
				}

				if (mFBO != 0U) {
					GL_ERR_CHECK(glDeleteFramebuffers(1, &mFBO));
					mFBO = 0;
				}
			}

			inline unsigned getHandle() const
			{
				return mFBO;
			}

			inline unsigned getTextureHandle() const
			{
				return mRBO;
			}

		private:
			unsigned mFBO;				
			unsigned mRBO;				/**< Renderbuffer that stores the depth and the stencil components of the image. */
			unsigned mColorBuffer;		/**< A texture that stores the color components of the image. */
		};

		class UnisampledFramebuffer
		{
		public:
			UnisampledFramebuffer(FramebufferProperties props, bool attachRenderbuffer = true) :
				mFBO(0U),
				mRBO(0U),
				mColorBuffer(0U)
			{
				// Generate and bind the framebuffer
				GL_ERR_CHECK(glGenFramebuffers(1, &mFBO));
				GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));

				// Generate, bind and reserve memory for the color buffer
				GL_ERR_CHECK(glGenTextures(1, &mColorBuffer));
				GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mColorBuffer));
				GL_ERR_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, props.Width, props.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

				// Set maximization and minimization filter for color buffer
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

				// Attach the color buffer object to the framebuffer object
				GL_ERR_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer, 0));

				if (attachRenderbuffer) {
					GL_ERR_CHECK(glGenRenderbuffers(1, &mRBO));
					GL_ERR_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, mRBO));
					GL_ERR_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, props.Width, props.Height));

					// Attach the render buffer
					GL_ERR_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO));
				}

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
					throw std::runtime_error("Unable to create framebuffer.");
				}
			}

			virtual ~UnisampledFramebuffer()
			{
				if (mColorBuffer != 0U) {
					GL_ERR_CHECK(glDeleteTextures(1, &mColorBuffer));
					mColorBuffer = 0U;
				}

				if (mRBO != 0) {
					GL_ERR_CHECK(glDeleteRenderbuffers(1, &mRBO));
					mRBO = 0;
				}

				if (mFBO != 0U) {
					GL_ERR_CHECK(glDeleteFramebuffers(1, &mFBO));
					mFBO = 0;
				}
			}

			inline unsigned getHandle() const
			{
				return mFBO;
			}

			inline unsigned getTextureHandle() const
			{
				return mColorBuffer;
			}

		private:
			unsigned mFBO;
			unsigned mRBO;				/**< Renderbuffer that stores the depth and the stencil components of the image. */
			unsigned mColorBuffer;		/**< A texture that stores the color components of the image. */
		};

	private:
		static std::string sVertexShader;
		static std::string sFragmentShader;

	private:
		std::unique_ptr<MultisampledFramebuffer> mMultisampledFramebuffer;	/**< The multisample framebuffer object. */
		std::unique_ptr<UnisampledFramebuffer> mUnisampledFrambuffer;			/**< A simple framebuffer. Framebuffer to perform multisampling blit. */
		
		bool mIsMSAAactivated;

		FramebufferProperties mProperties;

		std::shared_ptr<Shader> mScreenShader;

		std::shared_ptr<VertexArray> mQuadVAO;;
	};
};
