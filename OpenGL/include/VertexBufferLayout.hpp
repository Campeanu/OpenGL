#ifndef __VERTEXBUFFERLAYOUT_HPP_INCLUDED__
#define __VERTEXBUFFERLAYOUT_HPP_INCLUDED__

#include <GL/glew.h> // Always GLEW should be included before GLFW
#include <vector>


#include "Renderer.hpp"

struct VertexBufferElement
{
	unsigned int  type;
	unsigned int  count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: 		   return 4;
			case GL_UNSIGNED_INT:  return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}

};


class VertexBufferLayout {
public:
	VertexBufferLayout();
	virtual ~VertexBufferLayout();
	
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	/**
	 * Template specializations
	 */
	template<>
	void Push<float>(unsigned int count)
	{
		m_Element.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Element.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Element.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Element; }
	inline unsigned int GetStride() const& { return m_Stride; }

private:
	std::vector<VertexBufferElement> m_Element;
	unsigned int m_Stride;
};

#endif // !__VERTEXBUFFERLAYOUT_HPP_INCLUDED__
