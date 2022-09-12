#pragma once

#include "lhpch.h"
#include <glm/glm.hpp>

namespace Lighthouse
{
	enum ShaderType
	{
		FLAT_COLOR,
		TEXTURE,
	};

	struct ShaderSource
	{
		std::string vertexShaderSource;
		std::string fragmentShaderSource;
	};

	class Shader
	{
	public:
		Shader(ShaderType type);
		~Shader();

		unsigned int getProgramId();

		void bind() const;
		void unbind() const;

		void setUniform1i(const std::string& name, int v1);
		void setUniform2i(const std::string& name, int v1, int v2);
		void setUniform3i(const std::string& name, int v1, int v2, int v3);
		void setUniform4i(const std::string& name, int v1, int v2, int v3, int v4);

		void setUniform1ui(const std::string& name, unsigned int v1);
		void setUniform2ui(const std::string& name, unsigned int v1, unsigned int v2);
		void setUniform3ui(const std::string& name, unsigned int v1, unsigned int v2, unsigned int v3);
		void setUniform4ui(const std::string& name, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4);

		void setUniform1f(const std::string& name, float v1);
		void setUniform2f(const std::string& name, float v1, float v2);
		void setUniform3f(const std::string& name, float v1, float v2, float v3);
		void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);

		void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

	private:
		ShaderType _type;
		std::string _filepath;
		unsigned int _programId;
		std::unordered_map<std::string, int> _uniformLocationCache;

		void _initShader();
		ShaderSource _parseShaders(const std::string& filepath);
		unsigned int _createProgram(ShaderSource shaderSource);
		unsigned int _compileShader(unsigned int shaderType, const std::string& source);

		int _getUniformLocation(const std::string& name);
	};

}
