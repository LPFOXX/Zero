#pragma once

#include "Zero/Renderer/CubeMap.h"
#include "OpenGLShader.h"
#include "OpenGLVertexArray.h"
#include "Zero/Core/Core.h"
#include <glad/glad.h>

namespace zr
{
	class OpenGLCubeMap : public CubeMap
	{
	public:
		enum CubeFace
		{
			RightFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			LeftFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			TopFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			BottomFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			BackFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			FrontFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

	public:
		OpenGLCubeMap();

		/**
		* @brief Default destructor.
		*
		*/
		virtual ~OpenGLCubeMap();

		virtual bool loadFromFiles(const std::string& rightFace, const std::string& leftFace, const std::string& topFace, const std::string& bottomFace, const std::string& backFace, const std::string& frontFace) override;

		virtual bool loadFromFiles(const std::vector<std::string>& faces) override;

		virtual void setViewProjectionMatrix(const glm::mat4& viewProjectionMatrix) override;

		virtual unsigned getHandler() const override;

		virtual void render(const glm::mat4& viewProjectionMatrix) override;


		// Inherited via CubeMap
		virtual void bind() const override;
		virtual void unbind() const override;

	private:
		void genTexture();
		bool loadFace(CubeFace whichFace, const std::string& filePath);

	private:
		unsigned mTextureId;
		Ref<VertexArray> mVertexArray;
		Ref<Shader> mShader;
	};
}
