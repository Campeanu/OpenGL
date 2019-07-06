#ifndef __VERTEXARRAY_INCLUDED__
#define __VERTEXARRAY_INCLUDED__

#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
public:
	VertexArray();
	virtual ~VertexArray();
	
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddLayout();

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};


#endif // !__VERTEXARRAY_INCLUDED__