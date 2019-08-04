#include <zr_pch.h>

#include <glad/glad.h>

#include "Mesh.h"

namespace zr
{
	Mesh::Mesh(const MeshProperties& meshProps, std::vector<Vertex>& vertices, std::vector<unsigned>& indices, const std::vector<std::shared_ptr<Texture>>& textures) :
		mVAO(nullptr),
		mTextures(textures),
		mProperties(nullptr)
	{
		mProperties.reset(new MeshProperties(meshProps));
		mVAO.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> VBO;
		VBO.reset(VertexBuffer::Create((float*)& vertices[0].Position.x, vertices.size() * sizeof(Mesh::Vertex), DrawMode::Static));
		VBO->setLayout({
				{ ShaderDataType::Float3, "aPosition"},
				{ ShaderDataType::Float3, "aNormal"},
				{ ShaderDataType::Float3, "aTangent"},
				{ ShaderDataType::Float3, "aBitangent"},
				{ ShaderDataType::Float2, "aTexturesCoordinates"}
			});

		mVAO->addVertexBuffer(VBO);

		std::shared_ptr<IndexBuffer> EBO;
		EBO.reset(IndexBuffer::Create((unsigned*)& indices[0], indices.size(), DrawMode::Static));
		mVAO->setIndexBuffer(EBO);
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::render(const std::shared_ptr<Shader>& shader)
	{
		if (mProperties->Components & MeshProperties::BitField::Textures) {
			unsigned diffuse = 1;
			unsigned specular = 1;
			unsigned normal = 1;
			unsigned height = 1;

			for (unsigned i = 0; i < mTextures.size(); i++) {
				std::stringstream uniformName;
				switch (mTextures[i]->getType()) {
					case Texture::TextureType::None:
					{
						continue;
					}
					case Texture::TextureType::Diffuse:
					{
						uniformName << "texture_diffuse" << diffuse++;
						break;
					}
					case Texture::TextureType::Specular:
					{
						uniformName << "texture_specular" << specular++;
						break;
					}
					case Texture::TextureType::Normal:
					{
						uniformName << "texture_normal" << normal++;
						break;
					}
					case Texture::TextureType::Height:
					{
						uniformName << "texture_height" << height++;
						break;
					}
					default:
					break;
				}
				mTextures[i]->bindOnTextureUnit(i);
				shader->setUniform(uniformName.str().c_str(), static_cast<int>(i));

				mVAO->bind();
				glDrawElements(GL_TRIANGLES, mVAO->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
			}
		}
		else {
			shader->setUniform("uMaterial", mProperties->mMaterial);

			mVAO->bind();
			glDrawElements(GL_TRIANGLES, mVAO->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
		}
	}
}
