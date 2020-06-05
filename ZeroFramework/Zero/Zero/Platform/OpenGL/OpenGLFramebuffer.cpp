#include <zr_pch.h>

#include "GL_ERR_CHECK.h"
#include "Zero/Renderer/RenderCommand.h"
#include "Zero/Renderer/Renderer2D.h"
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

	OpenGLFramebuffer::OpenGLFramebuffer(const Properties& props) :
		Framebuffer(),
		mMultisampledFramebuffer(nullptr),
		mSinglesampledFrambuffer(nullptr),
		mIsMSAAactivated(false),
		mProperties(props),
		//mScreenShader(),
		//mQuadVAO(),
		mPixelSize(0.f, 0.f)
	{
		mProperties.setMaxMSAASamplesSupported(OpenGLFramebuffer::GetMaxSamples());
		mIsMSAAactivated = mProperties.getMSAASamples() > 1U;
		generateFramebuffer();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
	}

	void OpenGLFramebuffer::draw() const
	{
		/*blit();

		GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		mScreenShader->bind();
		mScreenShader->setUniform("screenTexture", 0);
		Texture2D::ActivateTextureSlot(0U, mSinglesampledFrambuffer->getTextureHandle());

		mQuadVAO->bind();
		RenderCommand::DrawIndexed(mQuadVAO);*/
	}

	void OpenGLFramebuffer::blit() const
	{
		if (mIsMSAAactivated) {
			GL_ERR_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, mMultisampledFramebuffer->getHandle()));
			GL_ERR_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mSinglesampledFrambuffer->getHandle()));
			GL_ERR_CHECK(glBlitFramebuffer(0, 0, mProperties.Width, mProperties.Height, 0, 0, mProperties.Width, mProperties.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
		}
	}

	void OpenGLFramebuffer::BindDefault()
	{
		GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	unsigned OpenGLFramebuffer::GetMaxSamples()
	{
		int maxSamples = 0;
		GL_ERR_CHECK(glGetIntegerv(GL_MAX_SAMPLES, &maxSamples));
		return maxSamples;
	}

	std::pair<unsigned, unsigned> OpenGLFramebuffer::GetMaxViewportSize()
	{
		int size[2];
		GL_ERR_CHECK(glGetIntegerv(GL_MAX_VIEWPORT_DIMS, size));
		return { (unsigned)size[0], (unsigned)size[1] };
	}

	void OpenGLFramebuffer::bind()
	{
		if (mIsMSAAactivated) {
			GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, mMultisampledFramebuffer->getHandle()));
		}
		else {
			GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, mSinglesampledFrambuffer->getHandle()));
		}
	}

	void OpenGLFramebuffer::unbind()
	{
		GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	unsigned OpenGLFramebuffer::getTextureHandle() const
	{
		return mSinglesampledFrambuffer->getTextureHandle();
	}

	const glm::vec2& OpenGLFramebuffer::getPixelSize() const
	{
		return mPixelSize;
	}

	const Framebuffer::Properties& OpenGLFramebuffer::getProperties() const
	{
		return mProperties;
	}

	void OpenGLFramebuffer::setSize(unsigned width, unsigned height)
	{
		if ((mProperties.Width != width || mProperties.Height != height) && (width > 0U && height > 0U)) {
			mProperties.Width = width;
			mProperties.Height = height;

			generateFramebuffer();
		}
	}

	unsigned OpenGLFramebuffer::setMSAASamples(unsigned msaaSamples)
	{
		mProperties.setMSAASamples(msaaSamples);
		if (mProperties.getMSAASamples() != (msaaSamples == 0 ? 1 : msaaSamples)) return mProperties.getMSAASamples();

		mIsMSAAactivated = mProperties.getMSAASamples() > 1U;
		generateFramebuffer();

		return mProperties.getMSAASamples();
	}

	void OpenGLFramebuffer::deleteBuffers()
	{
		mMultisampledFramebuffer.reset();
		mSinglesampledFrambuffer.reset();
	}

	void OpenGLFramebuffer::generateFramebuffer()
	{
		if (mIsMSAAactivated) {
			mMultisampledFramebuffer.reset(new MultisampledFramebuffer(mProperties));
			mSinglesampledFrambuffer.reset(new SinglesampledFramebuffer(mProperties, false));
		}
		else {
			mSinglesampledFrambuffer.reset(new SinglesampledFramebuffer(mProperties));
		}

		mPixelSize = { 1.f / mProperties.Width, 1.f / mProperties.Height };

		// Unbind the framebuffer
		GL_ERR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		//float quadVertices[]{
		//	1.f,  1.f,  0.f,		1.f, 1.f,		// top-right
		//	0.f,  1.f,  0.f,		0.f, 1.f,		// top-left
		//	0.f,  0.f,  0.f,		0.f, 0.f,		// bottom-left
		//	1.f,  0.f,  0.f,		1.f, 0.f		// bottom-right
		//};

		//GLuint quadIndices[]{
		//	0, 1, 2,		// first triangle
		//	2, 3, 0			// second triangle
		//};

		//mQuadVAO = VertexArray::Create();

		//Ref<VertexBuffer> VBO = VertexBuffer::Create(quadVertices, sizeof(quadVertices), DrawMode::Static);
		//VBO->setLayout({
		//	{ ShaderDataType::Float3, "a_Position" },
		//	{ ShaderDataType::Float2, "a_TextCoord" }
		//	});

		//mQuadVAO->addVertexBuffer(VBO);

		//Ref<IndexBuffer> EBO = IndexBuffer::Create(quadIndices, 6U, DrawMode::Static);
		//mQuadVAO->setIndexBuffer(EBO);

		//mScreenShader = Shader::Create();
		//if (!mScreenShader->loadFromStrings("OpenGLFramebufferScreenShader", sVertexShader, sFragmentShader)) {
		//	std::cout << "Error creating Shader\n";
		//}
	}
}
