//#include <zr_pch.h>
//
//#include <glad/glad.h>
//
//#include "Mesh.h"
//
//namespace zr
//{
//	Mesh::Mesh(const MeshData::MeshProperties& meshProps, std::vector<MeshData::Vertex>& vertices, std::vector<unsigned>& indices, const std::unordered_map<Texture2D::Type, std::vector<Image>>& textures) :
//		mVAO(nullptr),
//		mTextures(),
//		mProperties(nullptr)
//	{
//		mProperties.reset(new MeshData::MeshProperties(meshProps));
//		
//		for (auto& imageVector : textures) {
//			for (auto& image : imageVector.second) {
//				//Ref<Texture2D> texture = Texture2D::Create();
//				//texture.Create()
//				//mTextures.emplace_back(Texture2D())
//			}
//		}
//
//		mVAO = VertexArray::Create();
//
//		Ref<VertexBuffer> VBO = VertexBuffer::Create((float*)& vertices[0].Position.x, vertices.size() * sizeof(MeshData::Vertex), DrawMode::Static);
//		VBO->setLayout({
//				{ ShaderDataType::Float3, "aPosition"},
//				{ ShaderDataType::Float3, "aNormal"},
//				{ ShaderDataType::Float3, "aTangent"},
//				{ ShaderDataType::Float3, "aBitangent"},
//				{ ShaderDataType::Float2, "aTexturesCoordinates"}
//			});
//
//		mVAO->addVertexBuffer(VBO);
//
//		Ref<IndexBuffer> EBO = IndexBuffer::Create((unsigned*)& indices[0], indices.size(), DrawMode::Static);
//		mVAO->setIndexBuffer(EBO);
//	}
//
//	Mesh::~Mesh()
//	{
//
//	}
//
//	void Mesh::render(const Ref<Shader>& shader)
//	{
//		if (mProperties->Components & MeshData::MeshProperties::BitField::Textures) {
//			unsigned diffuse = 1;
//			unsigned specular = 1;
//			unsigned normal = 1;
//			unsigned height = 1;
//
//			for (unsigned i = 0; i < mTextures.size(); i++) {
//				std::stringstream uniformName;
//				switch (mTextures[i]->getType()) {
//					case Texture2D::Type::None:
//					{
//						continue;
//					}
//					case Texture2D::Type::Diffuse:
//					{
//						uniformName << "texture_diffuse" << diffuse++;
//						break;
//					}
//					case Texture2D::Type::Specular:
//					{
//						uniformName << "texture_specular" << specular++;
//						break;
//					}
//					case Texture2D::Type::Normal:
//					{
//						uniformName << "texture_normal" << normal++;
//						break;
//					}
//					case Texture2D::Type::Height:
//					{
//						uniformName << "texture_height" << height++;
//						break;
//					}
//					default:
//					break;
//				}
//				mTextures[i]->bindOnTextureUnit(i);
//				shader->setUniform(uniformName.str().c_str(), static_cast<int>(i));
//
//				mVAO->bind();
//				glDrawElements(GL_TRIANGLES, mVAO->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
//			}
//		}
//		else {
//			shader->setUniform("uMaterial", mProperties->mMaterial);
//
//			mVAO->bind();
//			glDrawElements(GL_TRIANGLES, mVAO->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
//		}
//	}
//}
