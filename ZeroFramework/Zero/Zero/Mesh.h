//#pragma once
//
//#include <unordered_map>
//#include "../vendor/glm/include/glm/glm.hpp"
//
//#include "Zero/Core/Core.h"
//#include "Zero/Renderer/Buffer.h"
//#include "Zero/Renderer/Texture.h"
//#include "Zero/Renderer/Shader.h"
//#include "Zero/Renderer/Material.h"
//
//#include "MeshData.h"
//
//namespace zr
//{
//	class Mesh
//	{
//	public:
//		Mesh(const MeshData::MeshProperties& meshProps, std::vector<MeshData::Vertex>& vertices, std::vector<unsigned>& indices, const std::unordered_map<Texture2D::Type, std::vector<Image>>& textures);
//		~Mesh();
//
//		void render(const Ref<Shader>& shader);
//
//	private:
//		Ref<VertexArray> mVAO;
//		std::vector<Ref<Texture2D>> mTextures;
//		std::unique_ptr<MeshData::MeshProperties> mProperties;
//	};
//}
