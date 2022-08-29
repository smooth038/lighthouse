#include "lhpch.h"

#include "Renderer.h"
#include "GL/glew.h"

namespace Lighthouse
{

	Renderer::Renderer()
	{
		float positions[6] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.0f,  0.5f
		};

		glewInit();

		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		std::ifstream shaderSource = std::ifstream("src\\Lighthouse\\Shaders\\default.shader");
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

		std::string vertexShaderSource = ss[0].str();
		std::string fragmentShaderSource = ss[1].str();
		const char* v = vertexShaderSource.c_str();
		const char* f = fragmentShaderSource.c_str();

		unsigned int program = glCreateProgram();
		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &v, nullptr);
		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &f, nullptr);
		glCompileShader(vs);
		glCompileShader(fs);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		glUseProgram(program);
	}

	void Renderer::testDrawTriangle()
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

}
