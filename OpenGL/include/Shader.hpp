#ifndef __SHADER_HPP_INCLUDED__
#define __SHADER_HPP_INCLUDED__

#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <unordered_map>

struct ShaderProgramSources
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
public:
	Shader(const std::string& filepath);
	virtual ~Shader();

	void Bind()   const;
	void Unbind() const;


	void SetUniform1i(const std::string& name, int   value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	// PRIVATE: Properties
private:
	std::string  m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	// PRIVATE: Methods
private:
	ShaderProgramSources ParseShader(const std::string& filepath);
	unsigned int CompileShaders(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	int GetUniformLocation(const std::string& name);

};

#endif // !__SHADER_HPP_INCLUDED__
