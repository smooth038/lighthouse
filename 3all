#include "lhpch.h"
#include "Shader.h"

#include "GL/glew.h"

namespace Lighthouse
{

	Shader::Shader(const std::string& filepath)
		: _filepath(filepath)
	{
		_programId = _createProgram(_parseShaders(_filepath));
	}

	Shader::~Shader()
	{
		glDeleteProgram(_programId);
	}

	void Shader::bind() const
	{
		glUseProgram(_programId);
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
	}

	void Shader::setUniform1i(const std::string& name, int v1)
	{
		glUniform1i(_getUniformLocation(name), v1);
	}

	void Shader::setUniform2i(const std::string& name, int v1, int v2)
	{
		glUniform2i(_getUniformLocation(name), v1, v2);
	}

	void Shader::setUniform3i(const std::string& name, int v1, int v2, int v3)
	{
		glUniform3i(_getUniformLocation(name), v1, v2, v3);
	}

	void Shader::setUniform4i(const std::string& name, int v1, int v2, int v3, int v4)
	{
		glUniform4i(_getUniformLocation(name), v1, v2, v3, v4);
	}

	void Shader::setUniform1ui(const std::string& name, unsigned int v1)
	{
		glUniform1ui(_getUniformLocation(name), v1);
	}

	void Shader::setUniform2ui(const std::string& name, unsigned int v1, unsigned int v2)
	{
		glUniform2ui(_getUniformLocation(name), v1, v2);
	}

	void Shader::setUniform3ui(const std::string& name, unsigned int v1, unsigned int v2, unsigned int v3)
	{
		glUniform3ui(_getUniformLocation(name), v1, v2, v3);
	}

	void Shader::setUniform4ui(const std::string& name, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4)
	{
		glUniform4ui(_getUniformLocation(name), v1, v2, v3, v4);
	}

	void Shader::setUniform1f(const std::string& name, float v1)
	{
		glUniform1f(_getUniformLocation(name), v1);
	}

	void Shader::setUniform2f(const std::string& name, float v1, float v2)
	{
		glUniform2f(_getUniformLocation(name), v1, v2);
	}

	void Shader::setUniform3f(const std::string& name, float v1, float v2, float v3)
	{
		glUniform3f(_getUniformLocation(name), v1, v2, v3);
	}

	void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
	{
		glUniform4f(_getUniformLocation(name), v1, v2, v3, v4);
	}

	void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(_getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	ShaderSource Shader::_parseShaders(const std::string& filepath)
	{
		std::ifstream shaderSource = std::ifstream(filepath);
		std::stringstream ss[2];
		std::string line;

		enum class ShaderType
		{
			NONE = -1,
			VERTEX = 0,
			FRAGMENT = 1,
		};

		ShaderType type = ShaderType::NONE;

		while (std::getline(shaderSource, line))
		{
			if (line.find("// Vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
				continue;
			}
			if (line.find("// Fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
				continue;
			}
			ss[(int)type] << line << std::endl;
		}

		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shader::_createProgram(ShaderSource shaderSource)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = _compileShader(GL_VERTEX_SHADER, shaderSource.vertexShaderSource);
		unsigned int fs = _compileShader(GL_FRAGMENT_SHADER, shaderSource.fragmentShaderSource);
		glAttachShader(program, vs);
		glAttachShader(program, fs);

		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	unsigned int Shader::_compileShader(unsigned int shaderType, const std::string& source)
	{
		unsigned int shader = glCreateShader(shaderType);
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) throw "Could not compile shader!";

		return shader;
	}

	int Shader::_getUniformLocation(const std::string& name)
	{
		if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
		{
			return _uniformLocationCache[name];
		}

		int location = glGetUniformLocation(_programId, name.c_str());
		if (location == -1) throw "Could not find specified uniform";

		_uniformLocationCache[name] = location;
		return location;
	}

}
