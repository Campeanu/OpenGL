#ifndef __VERTEXARRAY_INCLUDED__
#define __VERTEXARRAY_INCLUDED__

#include "VertexBuffer.hpp"

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	virtual ~VertexArray();
	
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};


#endif // !__VERTEXARRAY_INCLUDED__