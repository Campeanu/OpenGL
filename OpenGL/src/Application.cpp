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

/**
 * The use of this structure is for function ShaderProgramSource to be able tu return 2 strings 
 */
struct ShaderProgramSources 
{
	std::string VertexSource;
	std::string FragmentSource;
};

ShaderProgramSources ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE		= -1,
		VERTEX		=  0,
		FRAGMENT	=  1
	};

	std::string line;
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else 
				if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

  
unsigned int CompileShaders(unsigned int type, const std::string& source)
{
	GLCALL(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCALL((glShaderSource	(id, 1, &src, nullptr)));
	GLCALL(glCompileShader (id));

	// TODO: Error handling

	int result;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int lenght;
		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
		char* message = (char*)_malloca(lenght * sizeof(char)); // CONSIDERING USING alloca(...) insted _malloca
		GLCALL(glGetShaderInfoLog(id, lenght, &lenght, message));
		std::cout << "Failed to compile "<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment" ) << "shader: " << message << "\n";
		GLCALL(glDeleteShader(id));
		
		return 0;
	}

	return id;
}

unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCALL(unsigned int program	= glCreateProgram());
	unsigned int vs			= CompileShaders(GL_VERTEX_SHADER,   vertexShader  );
	unsigned int fs			= CompileShaders(GL_FRAGMENT_SHADER, fragmentShader);
	
	GLCALL(glAttachShader	  (program, vs));
	GLCALL(glAttachShader	  (program, fs));
	GLCALL(glLinkProgram	  (program));
	GLCALL(glValidateProgram  (program));
	GLCALL(glDeleteShader	  (vs));
	GLCALL(glDeleteShader	  (fs));

	return program;
}

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

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

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

		/**
		 * Vertex array
		 */
		unsigned int vao;
		GLCALL(glGenVertexArrays(1, &vao));
		GLCALL(glBindVertexArray(vao));

		/**
		 * VertexBuffer Abstraction
		 */
		VertexBuffer vb(position, 4 * 2 * sizeof(float));

		GLCALL(glEnableVertexAttribArray(0));
		GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

		/**
		 * IndexBuffer Abstraction
		 */
		IndexBuffer ib(indices, 6);

		ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");
		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		GLCALL(glUseProgram(shader));

		/**
		 * Link the uniform with owr shader and set the location of the uniform variable
		 */
		GLCALL(int location = glGetUniformLocation(shader, "u_Color"));
		ASSERT(location != -1);

		/**
		 * Add RGB atribute
		 */
		glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);

		GLCALL(glBindVertexArray(0));
		GLCALL(glUseProgram(0));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


		float r = 0.0f;
		float increment = 0.05f;

		std::cout << glGetString(GL_VERSION) << std::endl;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			GLCALL(glUseProgram(shader));
			glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

			GLCALL(glBindVertexArray(vao));
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
		GLCALL(glDeleteProgram(shader));
	}
	glfwTerminate(); 
}