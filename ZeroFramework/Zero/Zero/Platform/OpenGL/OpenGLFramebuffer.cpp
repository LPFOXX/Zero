#include <zr_pch.h>

#include "GL_ERR_CHECK.h"
#include "OpenGLFramebuffer.h"

namespace zr
{
	std::string OpenGLFramebuffer::sVertexShader = R"(
		#version 330 core
			
		layout(location=0) in vec3 a_Position;
		layout(location=1) in vec2 aTextCoord;

		out vec2 TextCoords;

		void main()
		{
			gl_Position = vec4(a_Position, 1.f);
			TextCoords = aTextCoord;
		}
	)";

	std::string OpenGLFramebuffer::sFragmentShader = R"(
		#version 330 core
	
		out vec4 FragColor;
		in vec2 TextCoords;
			
		uniform sampler2D screenTexture;

		void main()
		{
			FragColor = texture(screenTexture, TextCoords);
		}
	)";

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferProperties& props) :
		Framebuffer(),
		mMultisampledFramebuffer(nullptr),
		mUnisampledFrambuffer(nullptr),
		mIsMSAAactivated(props.MSSALevel > 0U),
		mProperties(props),
		mScreenShader(),
		mQuadVAO()
	{
		if (mIsMSAAactivated) {
			mMultisampledFramebuffer.reset(new MultisampledFramebuffer(props));
			mUnisampledFrambuffer.reset(new UnisampledFramebuffer(props, false));
		}
		else {
			mUnisampledFrambuffer.reset(new UnisampledFramebuffer(props));
		}

		// Unbind the framebuffer
		GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		float quadVertices[]{
			 1.f,  1.f,  0.f,		1.f, 1.f,		// top-right
			 0.f,  1.f,  0.f,		0.f, 1.f,		// top-left
			 0.f,  0.f,  0.f,		0.f, 0.f,		// bottom-left
			 1.f,  0.f,  0.f,		1.f, 0.f		// bottom-right
		};

		GLuint quadIndices[]{
			0, 1, 2,		// first triangle
			2, 3, 0			// second triangle
		};

		mQuadVAO = VertexArray::Create();

		Ref<VertexBuffer> VBO = VertexBuffer::Create(quadVertices, sizeof(quadVertices), DrawMode::Static);
		VBO->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TextCoord" }
			});

		mQuadVAO->addVertexBuffer(VBO);

		Ref<IndexBuffer> EBO = IndexBuffer::Create(quadIndices, 6U, DrawMode::Static);
		mQuadVAO->setIndexBuffer(EBO);

		mScreenShader = Shader::Create();
		if (!mScreenShader->loadFromStrings("OpenGLFramebufferScreenShader", sVertexShader, sFragmentShader)) {
			std::cout << "Error creating Shader\n";
		}
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
	}

	void OpenGLFramebuffer::draw() const
	{
		if (mIsMSAAactivated) {
			GL_ERR_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, mMultisampledFramebuffer->getHandle()));
			GL_ERR_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mUnisampledFrambuffer->getHandle()));
			GL_ERR_CHECK(glBlitFramebuffer(0, 0, mProperties.Width, mProperties.Height, 0, 0, mProperties.Width, mProperties.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST));

			GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}

		mScreenShader->bind();
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mUnisampledFrambuffer->getTextureHandle()));
		GL_ERR_CHECK(glActiveTexture(GL_TEXTURE0));
		mScreenShader->setUniform("screenTexture", 0);

		mQuadVAO->bind();
		GL_ERR_CHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	}

	void OpenGLFramebuffer::BindDefault()
	{
		GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void OpenGLFramebuffer::bind()
	{
		if (mIsMSAAactivated) {
			GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, mMultisampledFramebuffer->getHandle()));
		}
		else {
			GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, mUnisampledFrambuffer->getHandle()));
		}
	}

	void OpenGLFramebuffer::unbind()
	{
		GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
}
