#include <zr_pch.h>

#include <algorithm>
#include <glad/glad.h>

#include "Mesh.h"
#include "Renderer/RenderCommand.h"
#include "ImGui/ImGuiConsole.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <fstream>

namespace zr
{
	//Mesh::Mesh(const MeshData::MeshProperties& meshProps, std::vector<MeshData::Vertex>& vertices, std::vector<unsigned>& indices, const std::unordered_map<Texture2D::Type, std::vector<Image>>& textures) :
	//	mVAO(nullptr),
	//	mTextures(),
	//	mProperties(nullptr)
	//{
	//	mProperties.reset(new MeshData::MeshProperties(meshProps));
	//	
	//	for (auto& imageVector : textures) {
	//		for (auto& image : imageVector.second) {
	//			//Ref<Texture2D> texture = Texture2D::Create();
	//			//texture.Create()
	//			//mTextures.emplace_back(Texture2D())
	//		}
	//	}

	//	mVAO = VertexArray::Create();

	//	Ref<VertexBuffer> VBO = VertexBuffer::Create((float*)& vertices[0].Position.x, vertices.size() * sizeof(MeshData::Vertex), DrawMode::Static);
	//	VBO->setLayout({
	//			{ ShaderDataType::Float3, "aPosition"},
	//			{ ShaderDataType::Float3, "aNormal"},
	//			{ ShaderDataType::Float3, "aTangent"},
	//			{ ShaderDataType::Float3, "aBitangent"},
	//			{ ShaderDataType::Float2, "aTexturesCoordinates"}
	//		});

	//	mVAO->addVertexBuffer(VBO);

	//	Ref<IndexBuffer> EBO = IndexBuffer::Create((unsigned*)& indices[0], indices.size(), DrawMode::Static);
	//	mVAO->setIndexBuffer(EBO);
	//}

	Mesh::Mesh(MeshData& meshData) :
		mVAO(nullptr),
		mTextures(),
		mProperties(meshData.getProperties())
	{
		ZR_IMGUI_LOG(ConsoleItem::Info, "Mesh name: %s", mProperties.Name.c_str());
		/*auto& textures = meshData.getTextures();
		ZR_CORE_INFO("\tTextures");
		for (auto& t : textures) {
			ZR_CORE_INFO("\tTexture type {0}: ", Texture2D::typeToString(t.first));
			for (auto& imagePath : t.second) {
				ZR_CORE_INFO("\t\tImage path: {0}", imagePath);
			}
		}*/
		
		mVAO = VertexArray::Create();
		std::vector<MeshData::Vertex>& v = meshData.getVertices();
		Ref<VertexBuffer> VBO = VertexBuffer::Create(&v[0].Position.x, v.size() * sizeof(MeshData::Vertex), DrawMode::Static);
		
		BufferLayout bl;
		bl.addElement(BufferElement(ShaderDataType::Float3, "aPosition"));

		if (mProperties.Components & MeshData::Normals) {
			bl.addElement(BufferElement(ShaderDataType::Float3, "aNormal"));
		}

		if (mProperties.Components & MeshData::TangentsAndBitangents) {
			bl.addElement(BufferElement(ShaderDataType::Float3, "aTangent"));
			bl.addElement(BufferElement(ShaderDataType::Float3, "aBitangent"));
		}

		if (mProperties.Components & MeshData::TextureCoordinates) {
			bl.addElement(BufferElement(ShaderDataType::Float2, "aTexturesCoordinates"));
		}

		VBO->setLayout(bl);
		mVAO->addVertexBuffer(VBO);

		std::vector<unsigned>& i = meshData.getIndices();
		Ref<IndexBuffer> EBO = IndexBuffer::Create(&i[0], i.size(), DrawMode::Static);
		mVAO->setIndexBuffer(EBO);
	}

	Mesh::Mesh(MeshData& meshData, std::vector<Ref<Texture2D>>& modelTextures) :
		mVAO(nullptr),
		mTextures(),
		mProperties(meshData.getProperties())
	{
		/*ZR_IMGUI_LOG(ConsoleItem::Info, "Mesh name: %s", mProperties.Name.c_str());
		auto& meshTextures = meshData.getTextures();
		ZR_IMGUI_LOG(ConsoleItem::Info, "\tTextures");
		for (auto& t : meshTextures) {
			ZR_IMGUI_LOG(ConsoleItem::Info, "\tTexture type: %s", Texture2D::typeToString(t.first).c_str());
			for (auto& imagePath : t.second) {
				ZR_IMGUI_LOG(ConsoleItem::Info, "\t\tImage path: %s", imagePath.c_str());
			}
		}*/

		mVAO = VertexArray::Create();
		MeshData::VertexSeparated& v = meshData.getSeparatedVertices();

		if (v.VertexData.find(MeshData::Positions) != v.VertexData.end()) {
			BufferLayout positionLayout;
			positionLayout.addElement(BufferElement(ShaderDataType::Float3, "aPosition"));
			std::vector<float>& p = v.VertexData[MeshData::Positions];
			Ref<VertexBuffer>& positionVBO = VertexBuffer::Create(&p[0], p.size() * sizeof(float), DrawMode::Static);
			positionVBO->setLayout(positionLayout);
			mVAO->addVertexBuffer(positionVBO);
		}

		if (mProperties.Components & MeshData::Components::Normals) {
			BufferLayout bl;
			bl.addElement(BufferElement(ShaderDataType::Float3, "aNormal"));
			std::vector<float>& p = v.VertexData[MeshData::Normals];
			Ref<VertexBuffer>& normalVBO = VertexBuffer::Create(&p[0], p.size() * sizeof(float), DrawMode::Static);
			normalVBO->setLayout(bl);
			mVAO->addVertexBuffer(normalVBO);
		}

		if (mProperties.Components & MeshData::Components::TangentsAndBitangents) {
			BufferLayout bl;
			bl.addElement(BufferElement(ShaderDataType::Float3, "aTangent"));
			bl.addElement(BufferElement(ShaderDataType::Float3, "aBitangent"));
			std::vector<float>& p = v.VertexData[MeshData::TangentsAndBitangents];
			Ref<VertexBuffer>& tangentsAndBitangentsVBO = VertexBuffer::Create(&p[0], p.size() * sizeof(float), DrawMode::Static);
			tangentsAndBitangentsVBO->setLayout(bl);
			mVAO->addVertexBuffer(tangentsAndBitangentsVBO);
		}

		if (mProperties.Components & MeshData::Components::TextureCoordinates) {
			BufferLayout bl;
			bl.addElement(BufferElement(ShaderDataType::Float2, "aTexturesCoordinates"));
			std::vector<float>& p = v.VertexData[MeshData::TextureCoordinates];
			Ref<VertexBuffer>& textCoordinatesVBO = VertexBuffer::Create(&p[0], p.size() * sizeof(float), DrawMode::Static);
			textCoordinatesVBO->setLayout(bl);
			mVAO->addVertexBuffer(textCoordinatesVBO);
		}
		
		if (mProperties.Components & MeshData::Components::Animations) {
			unsigned attributeLength = (v.MaxBonesPerVertex / 4) + (v.MaxBonesPerVertex % 4 == 0 ? 0 : 1);

			std::vector<unsigned> boneIndices;
			std::vector<float> boneWeights;

			for (auto& vertexBoneData : v.BoneData) {
				unsigned currentVertexBoneCount = vertexBoneData.second.BoneIndices.size();
				unsigned fillWithZeros = attributeLength * 4 - currentVertexBoneCount;

				for (unsigned i = 0; i < currentVertexBoneCount; ++i) {
					boneIndices.push_back(vertexBoneData.second.BoneIndices[i]);
					boneWeights.push_back(vertexBoneData.second.BoneWeights[i]);
				}
				for (unsigned i = 0; i < fillWithZeros; ++i) {
					boneIndices.push_back(0);
					boneWeights.push_back(0.f);
				}
			}

			BufferLayout boneIndicesLayout;
			BufferLayout boneWeightsLayout;
			boneIndicesLayout.addElement(BufferElement(ShaderDataType::Int4, "aBoneIndeces", attributeLength));
			boneWeightsLayout.addElement(BufferElement(ShaderDataType::Float4, "aBoneWeights", attributeLength));

			Ref<VertexBuffer>& boneIndicesVBO = VertexBuffer::Create(&boneIndices[0], boneIndices.size() * sizeof(unsigned), DrawMode::Static);
			boneIndicesVBO->setLayout(boneIndicesLayout);
			mVAO->addVertexBuffer(boneIndicesVBO);

			Ref<VertexBuffer>& boneWeightsVBO = VertexBuffer::Create(&boneWeights[0], boneWeights.size() * sizeof(float), DrawMode::Static);
			boneWeightsVBO->setLayout(boneWeightsLayout);
			mVAO->addVertexBuffer(boneWeightsVBO);

			/*std::ofstream file(mProperties.Name + std::to_string(v.MaxBonesPerVertex) + ".txt");
			file << "Max Bones per vertex " << v.MaxBonesPerVertex << '\n';
			unsigned vertexIndex = 0;
			for (unsigned i = 0; i < boneIndices.size(); ++i) {
				file << boneIndices[i] << "-->" << boneWeights[i] << "  ";
				if ((i + 1) % (4 * attributeLength) == 0) {
					file << '(' << v.BoneData[vertexIndex].BoneIndices.size() << "--" << v.BoneData[vertexIndex].BoneWeights.size() << ")[" << vertexIndex << "]\n";
					++vertexIndex;
				}
			}
			file.close();*/
		}

		Ref<IndexBuffer>& EBO = IndexBuffer::Create(&v.Indices[0], v.Indices.size(), DrawMode::Static);
		mVAO->setIndexBuffer(EBO);

		// Map textures
		auto& textures = meshData.getTextures();
		for (auto& t : textures) {
			for (auto& imagePath : t.second) {
				auto& it = std::find_if(modelTextures.begin(), modelTextures.end(), [&imagePath](Ref<Texture2D>& textureRef) {
					return textureRef->getFilePath() == imagePath;
				});

				if (it != modelTextures.end()) {
					mTextures[t.first].emplace_back(*it);
				}
			}
		}
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::render(const Ref<Shader>& shader)
	{
		/*if (mProperties.Components & MeshData::MeshProperties::BitField::Textures) {
			unsigned diffuse = 1;
			unsigned specular = 1;
			unsigned normal = 1;
			unsigned height = 1;

			for (unsigned i = 0; i < mTextures.size(); i++) {
				std::stringstream uniformName;
				switch (mTextures[i]->getType()) {
					case Texture2D::Type::None:
					{
						continue;
					}
					case Texture2D::Type::Diffuse:
					{
						uniformName << "texture_diffuse" << diffuse++;
						break;
					}
					case Texture2D::Type::Specular:
					{
						uniformName << "texture_specular" << specular++;
						break;
					}
					case Texture2D::Type::Normal:
					{
						uniformName << "texture_normal" << normal++;
						break;
					}
					case Texture2D::Type::Height:
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
			shader->setUniform("uMaterial", mProperties.mMaterial);

			mVAO->bind();
			glDrawElements(GL_TRIANGLES, mVAO->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
		}*/
		mVAO->bind();
		RenderCommand::DrawIndexed(mVAO);
	}

	unsigned Mesh::getVertexArrayMaxLayoutIndex() const
	{
		auto& vertexBuffers = mVAO->getVertexBuffers();
		auto& it = std::max_element(vertexBuffers.begin(), vertexBuffers.end(), [](const Ref<VertexBuffer>& a, const Ref<VertexBuffer>& b) {
			return a->getMaxLayoutIndex() > b->getMaxLayoutIndex();
		});
		return (*it)->getMaxLayoutIndex();
	}

	std::string Mesh::getShaderLayoutLocation() const
	{
		return mVAO->getShaderLayouts();
	}
}
