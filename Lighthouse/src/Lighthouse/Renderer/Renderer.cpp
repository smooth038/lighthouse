#include "lhpch.h"

#include "Renderer.h"
#include "GL/glew.h"

namespace Lighthouse
{

	static Scene _scene;

	void Renderer::init()
	{
		glewExperimental = GL_TRUE;
		glewInit();

		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		unsigned int ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

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

	void Renderer::addEntity(std::string id, std::vector<float> vertices, std::vector<unsigned int> indices)
	{
		Entity e(id);
		e.addVertices(vertices);
		e.addIndices(indices);
		_scene.addEntity(e);
	}

	void Renderer::drawTriangle()
	{
		float vertices[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.0f,  0.5f,

			 0.7f,  0.0f,
			 0.9f,  0.0f,
			 0.8f,  0.2f
		};

		unsigned int indices[] = {
			0, 1, 2,
			3, 4, 5
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		float vertices2[] = {
			-0.9f,  0.7f,
			-0.7f,  0.7f,
			-0.7f,  0.9f,
			-0.9f,  0.9f,
		};

		unsigned int indices2[] = {
			0, 1, 2,
			0, 2, 3
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::renderScene()
	{
		_scene.render();
	}

}
