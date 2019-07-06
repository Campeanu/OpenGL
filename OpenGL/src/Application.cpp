#include <GL/glew.h> // Always GLEW should be included before GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"

#include "Shader.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) return -1;

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	//glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window =  glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); /* Make the window's context current */

	glfwSwapInterval(1); // Add a limit to frame rate
	
	if (glewInit() != GLEW_OK) std::cout << "Error!" << std::endl;

	{
		float position[] = {
			-0.5f, -0.5f, // 0
			 0.5f, -0.5f, // 1
			 0.5f,  0.5f, // 2
			-0.5f,  0.5f  // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		unsigned int vao;
		GLCALL(glGenVertexArrays(1, &vao));
		GLCALL(glBindVertexArray(vao));

		VertexArray  va;
		VertexBuffer vb(position, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();		

		float r = 0.0f;
		float increment = 0.05f;

		std::cout << glGetString(GL_VERSION) << std::endl;

		while (!glfwWindowShouldClose(window))
		{
			GLCALL(glClear(GL_COLOR_BUFFER_BIT));

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			va.Bind();
			ib.Bind();

			GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

			/**
			 * Add an animation
			 */
			if (r > 1.0f)
				increment = -0.05f;
			else
				if (r < 0.0f)
					increment = +0.05f;

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate(); 
}