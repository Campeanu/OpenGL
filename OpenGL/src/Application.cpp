#include <GL/glew.h> // Always GLEW should be included before GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSources 
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSources ParseShader(const std::string& filepath)
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

  
static unsigned int CompileShaders(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource	(id, 1, &src, nullptr);
	glCompileShader (id);

	// TODO: Error handling

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)_malloca(lenght * sizeof(char)); // CONSIDERING USING alloca(...) insted _malloca
		glGetShaderInfoLog(id, lenght, &lenght, message);
		std::cout << "Failed to compile "<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment" ) << "shader: " << message << "\n";
		glDeleteShader(id);
		
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program	= glCreateProgram();
	unsigned int vs			= CompileShaders(GL_VERTEX_SHADER,   vertexShader  );
	unsigned int fs			= CompileShaders(GL_FRAGMENT_SHADER, fragmentShader);
	
	glAttachShader	  (program, vs);
	glAttachShader	  (program, fs);
	glLinkProgram	  (program);
	glValidateProgram (program);
	glDeleteShader	  (vs);
	glDeleteShader	  (fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) return -1;

	/* Create a windowed mode window and its OpenGL context */
	window =  glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	
	if (glewInit() != GLEW_OK) std::cout << "Error!" << std::endl;


	float position[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	float position2[6] = {
	-0.5f, 0.5f,
	 0.5f, 0.5f,
	 0.5f, -0.5f
	};

	unsigned int buffer;

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");

	//std::cout << "VERTEX: " << std::endl;
	//std::cout << source.VertexSource << std::endl;
	//std::cout << "FRAGMENT: " << std::endl;
	//std::cout << source.FragmentSource << std::endl;

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Draw the triangle */
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
}