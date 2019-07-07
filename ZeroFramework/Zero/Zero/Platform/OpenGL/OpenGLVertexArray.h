#include "Zero/Renderer/Buffer.h"

namespace zr
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		// Inherited via VertexArray
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		virtual void setLayout(const BufferLayout&) override;
		virtual void bind() override;
		virtual void unbind() override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override;
		virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override;

	private:
		unsigned mId;
		unsigned mAttributesBound;
		std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
	};
}
