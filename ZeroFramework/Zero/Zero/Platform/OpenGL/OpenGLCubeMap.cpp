#include <zr_pch.h>

#include "GL_ERR_CHECK.h"
#include "OpenGLCubeMap.h"
#include "OpenGLBuffer.h"
#include "OpenGLTexture.h"

#include "../../Renderer/Image.h" 
#include "../../Renderer/ImageReader.h" 

namespace zr
{
	OpenGLCubeMap::OpenGLCubeMap() :
		mTextureId(0U)
	{
		float skyboxVertices[]{
			// positions
			 1.f,  1.f, -1.f,		// 0
			 1.f, -1.f, -1.f,		// 1
			-1.f, -1.f, -1.f,		// 2
			-1.f,  1.f, -1.f,		// 3
			-1.f,  1.f,  1.f,		// 4
			-1.f, -1.f,  1.f,		// 5
			 1.f, -1.f,  1.f,		// 6
			 1.f,  1.f,  1.f		// 7
		};

		unsigned indices[]{
			// Back face
			0, 1, 2,
			0, 2, 3,

			// Left face
			3, 2, 5,
			3, 5, 4,

			// Front face
			4, 5, 6,
			4, 6, 7,

			// Right face
			7, 6, 1,
			7, 1, 0,

			// Top face
			3, 4, 7,
			3, 7, 0,

			// Bottom face
			5, 2, 1,
			5, 1, 6
		};

		mVertexArray.reset(new OpenGLVertexArray);

		std::shared_ptr<OpenGLVertexBuffer> vertexBuffer;
		vertexBuffer.reset(new OpenGLVertexBuffer(skyboxVertices, sizeof(skyboxVertices), DrawMode::Static));
		vertexBuffer->setLayout({
			{ShaderDataType::Float3, "aPosition"}
			});
		mVertexArray->addVertexBuffer(vertexBuffer);

		std::shared_ptr<OpenGLIndexBuffer> indexBuffer;
		indexBuffer.reset(new OpenGLIndexBuffer(indices, 6 * 6, DrawMode::Static));
		mVertexArray->setIndexBuffer(indexBuffer);

		std::string vertexShader = R"(
			#version 330 core

			layout (location = 0) in vec3 aPosition;

			uniform mat4 uViewProjection;

			out vec3 TexCoords;

			void main()
			{
				// The texture coordinates of the images are the vertices of
				// the cube.
				TexCoords = aPosition;
				vec4 pos = uViewProjection * vec4(aPosition, 1.0);
				gl_Position = pos.xyww;
			}
		)";

		std::string fragmentShader = R"(
			#version 330 core
			
			// Instead of a sampler2D a samplerCube is used
			uniform samplerCube uSkybox;

			out vec4 FragColor;
			in vec3 TexCoords;

			void main()
			{
				FragColor = texture(uSkybox, TexCoords);
			}
		)";

		mShader.reset(new OpenGLShader());
		if (!mShader->loadFromStrings(vertexShader, fragmentShader)) {
			ZR_CORE_ERROR("Failed to load cubemap shader.");
		}
	}

	OpenGLCubeMap::~OpenGLCubeMap()
	{
		if (mTextureId != 0U) {
			GL_ERR_CHECK(glDeleteTextures(1, &mTextureId));
			mTextureId = 0U;
		}
	}

	bool OpenGLCubeMap::loadFromFiles(const std::string& rightFace, const std::string& leftFace, const std::string& topFace, const std::string& bottomFace, const std::string& backFace, const std::string& frontFace)
	{
		genTexture();

		return
			loadFace(RightFace, rightFace) &&
			loadFace(LeftFace, leftFace) &&
			loadFace(TopFace, topFace) &&
			loadFace(BottomFace, bottomFace) &&
			loadFace(BackFace, backFace) &&
			loadFace(FrontFace, frontFace);
	}

	bool OpenGLCubeMap::loadFromFiles(const std::vector<std::string>& faces)
	{
		// Generate and bind the cubemap texture
		GL_ERR_CHECK(glGenTextures(1, &mTextureId));
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId));

		std::unique_ptr<unsigned char> data;
		for (unsigned i = 0; i < faces.size(); i++) {
			int width, height, nrChannels;
			data.reset(ImageReader::LoadDataFromFile(faces[i], width, height, nrChannels, false));
			if (data) {
				GL_ERR_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get()));
				//ImageReader::cleanData(data.get());
			}
			else {
				ZR_CORE_ERROR("Failed to load texture: {0}", faces[i]);
				return false;
			}
		}

		GL_ERR_CHECK(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
		// Set minifying and magnifying filters as linear 
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		return true;
	}

	void OpenGLCubeMap::setViewProjectionMatrix(const glm::mat4& viewProjectionMatrix)
	{
		mShader->bind();
		mShader->setUniform("uViewProjection", viewProjectionMatrix);
	}

	unsigned OpenGLCubeMap::getHandler() const
	{
		return mTextureId;
	}

	void OpenGLCubeMap::render(const glm::mat4& viewProjectionMatrix)
	{
		mShader->bind();
		mShader->setUniform("uViewProjection", viewProjectionMatrix);
		mShader->setUniform("uSkybox", 0);
		mVertexArray->bind();
		
		GL_ERR_CHECK(glActiveTexture(GL_TEXTURE0));
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId));
		GL_ERR_CHECK(glDrawElements(GL_TRIANGLES, mVertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLCubeMap::genTexture()
	{
		
	}

	bool OpenGLCubeMap::loadFace(CubeFace whichFace, const std::string& filePath)
	{
		int width, height, nrChannels;
		unsigned char* data = ImageReader::LoadDataFromFile(filePath, width, height, nrChannels, false);

		if (data) {
			GL_ERR_CHECK(glTexImage2D(whichFace, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
			GL_ERR_CHECK(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
			ImageReader::CleanData(data);
			return true;
		}
		else {
			return false;
		}
	}

	void OpenGLCubeMap::bind() const
	{
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId));
	}

	void OpenGLCubeMap::unbind() const
	{
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
}
