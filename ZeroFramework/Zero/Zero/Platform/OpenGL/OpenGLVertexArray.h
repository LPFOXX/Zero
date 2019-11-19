#include "Zero/Renderer/Buffer.h"

namespace zr
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray(bool createAndBind = false);
		virtual ~OpenGLVertexArray();

		// Inherited via VertexArray
		virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
		virtual void setLayout(const BufferLayout&) override;
		virtual void bind() override;
		virtual void unbind() override;

		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override;
		virtual const Ref<IndexBuffer>& getIndexBuffer() const override;

		virtual std::string getShaderLayouts() const override;
		virtual unsigned computeNextIndexLocation() const;

	private:
		unsigned mId;
		std::vector<Ref<VertexBuffer>> mVertexBuffers;
		Ref<IndexBuffer> mIndexBuffer;
	};
}
