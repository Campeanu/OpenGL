#ifndef __RENDERER_HPP_INCLUDED__
#define __RENDERER_HPP_INCLUDED__

#include <GL/glew.h> // Always GLEW should be included before GLFW

#include <iostream>

#include "Shader.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/**
 * Managing OpenGL errors 
 */
void GLClearError();

/**
 * Managing OpenGL errors 
 */
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
	void Clear();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);	

private:
};

#endif // !__RENDERER_HPP_INCLUDED__