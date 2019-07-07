#include "Shader.hpp"
#include "Renderer.hpp"

Shader::Shader(const std::string& filepath)
	: m_FilePath { filepath }, m_RendererID{ 0 }
{

	ShaderProgramSources source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	
}

Shader::~Shader()
{
	GLCALL(glDeleteProgram(m_RendererID));
}

ShaderProgramSources Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
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

unsigned int Shader::CompileShaders(unsigned int type, const std::string& source)
{
	GLCALL(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCALL((glShaderSource(id, 1, &src, nullptr)));
	GLCALL(glCompileShader(id));

	// TODO: Error handling

	int result;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int lenght;
		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
		char* message = (char*)_malloca(lenght * sizeof(char)); // CONSIDERING USING alloca(...) insted _malloca
		GLCALL(glGetShaderInfoLog(id, lenght, &lenght, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader: " << message << "\n";
		GLCALL(glDeleteShader(id));

		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCALL(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShaders(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShaders(GL_FRAGMENT_SHADER, fragmentShader);

	GLCALL(glAttachShader(program, vs));
	GLCALL(glAttachShader(program, fs));
	GLCALL(glLinkProgram(program));
	GLCALL(glValidateProgram(program));
	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	return program;
}


void Shader::Bind()   const
{
	GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCALL(glUseProgram(0));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
	GLCALL(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	
	m_UniformLocationCache[name] = location;
	return location;
}