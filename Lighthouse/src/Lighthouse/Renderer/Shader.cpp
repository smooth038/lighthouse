#include "lhpch.h"
#include "Shader.h"

#include <GL/glew.h>

namespace Lighthouse
{

	Shader::Shader(ShaderType type)
		: _type(type)
	{
		_initShader();
		_programId = _createProgram(_parseShaders(_filepath));
	}

	Shader::~Shader()
	{
		glDeleteProgram(_programId);
	}

	unsigned int Shader::getProgramId()
	{
		return _programId;
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

	void Shader::setUniform1fv(const std::string& name, float v1)
	{
		glUniform1fv(_getUniformLocation(name), 1, &v1);
	}

	void Shader::setUniform2fv(const std::string& name, float v1, float v2)
	{
		float vec2[2] = {v1, v2};
		glUniform2fv(_getUniformLocation(name), 2, vec2);
	}

	void Shader::setUniform3fv(const std::string& name, float v1, float v2, float v3)
	{
		float vec3[3] = {v1, v2, v3};
		glUniform3fv(_getUniformLocation(name), 3, vec3);
	}

	void Shader::setUniform4fv(const std::string& name, float v1, float v2, float v3, float v4)
	{
		float vec4[4] = {v1, v2, v3, v4};
		glUniform4fv(_getUniformLocation(name), 4, vec4);
	}

	void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(_getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::_initShader()
	{
		switch (_type)
		{
		case FLAT_COLOR:
			_filepath = "..\\Lighthouse\\src\\Lighthouse\\Shaders\\flatColor.shader";
			// positions
			glVertexAttribPointer(0, 3, GL_FLOAT, false, 7 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			// colors
			glVertexAttribPointer(1, 4, GL_FLOAT, false, 7 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			break;
		case TEXTURE:
			_filepath = "..\\Lighthouse\\src\\Lighthouse\\Shaders\\texture.shader";
			// positions
			glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			// texture coordinates
			glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// normals (per vertex smooth shading)
			glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));
			glEnableVertexAttribArray(2);
			break;
		case HIGHLIGHT:
			_filepath = "..\\Lighthouse\\src\\Lighthouse\\Shaders\\highlight.shader";
			// positions
			glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			// texture coordinates
			glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// normals (per vertex smooth shading)
			glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));
			glEnableVertexAttribArray(2);
			break;
		case PICKING:
			_filepath = "..\\Lighthouse\\src\\Lighthouse\\Shaders\\picking.shader";
			// positions
			glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			// texture coordinates
			glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// normals (per vertex smooth shading)
			glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));
			glEnableVertexAttribArray(2);
			break;

		}
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

		shaderSource.close();
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
		if (result == GL_FALSE)
		{
			LH_CORE_ERROR("Could not compile shader: {0}", source);
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetShaderInfoLog(shader, 1024, &log_length, message);
			LH_CORE_ERROR(message);
		}

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			LH_CORE_ERROR("Error detected: {0}", err);
		}

		return shader;
	}

	int Shader::_getUniformLocation(const std::string& name)
	{
		if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
		{
			return _uniformLocationCache[name];
		}

		//GLint i;
		//GLint count;

		//GLint size; // size of the variable
		//GLenum type; // type of the variable (float, vec3 or mat4, etc)

		//const GLsizei bufSize = 24; // maximum name length
		//GLchar name2[bufSize]; // variable name in GLSL
		//GLsizei length; // name length
		//glGetProgramiv(_programId, GL_ACTIVE_UNIFORMS, &count);
		//printf("Active Uniforms: %d\n", count);

		//for (int i = 0; i < count; i++)
		//{
		//	glGetActiveUniform(_programId, (GLuint)i, bufSize, &length, &size, &type, name2);
		//	printf("Uniform #%d Type: %u Name: %s\n", i, type, name2);
		//}

		int location = glGetUniformLocation(_programId, name.c_str());

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			LH_CORE_ERROR("Error detected: {0}", err);
		}

		if (location == -1)
		{
			LH_CORE_ERROR("Could not find specified uniform: {0}", name);
			__debugbreak();
		}

		_uniformLocationCache[name] = location;
		return location;
	}

}
