#pragma once

#include "../vendor/glm/include/glm/glm.hpp"
#include "Renderer/Buffer.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

namespace zr
{
	class Mesh
	{
	public:
		struct Vertex
		{
			Vertex() :
				Position(0.f, 0.f, 0.f),
				Normal(0.f, 0.f, 0.f),
				Tangent(0.f, 0.f, 0.f),
				Bitangent(0.f, 0.f, 0.f),
				TextureCoordinates(0.f, 0.f)
			{

			}

			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
			glm::vec2 TextureCoordinates;
		};
		
		struct MeshProperties
		{
			MeshProperties() :
				Name(""),
				HasNormals(false),
				HasTangentsAndBitangents(false),
				HasTextureCoordinates(false)
			{

			}

			std::string Name;
			bool HasNormals;
			bool HasTangentsAndBitangents;
			bool HasTextureCoordinates;
		};	


	public:
		Mesh(const MeshProperties& meshProps, std::vector<Vertex>& vertices, std::vector<unsigned>& indices, const std::vector<std::shared_ptr<Texture>>& textures);
		~Mesh();

		void render(const std::shared_ptr<Shader>& shader);

	private:
		std::shared_ptr<VertexArray> mVAO;
		std::vector<std::shared_ptr<Texture>> mTextures;
		MeshProperties mProperties;
	};
}
