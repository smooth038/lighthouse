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

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 7 * sizeof(float), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, false, 7 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

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

	Entity* Renderer::addEntity(std::string id, std::vector<float> vertices, std::vector<unsigned int> indices)
	{
		Entity e(id);
		e.addVertices(vertices);
		e.addIndices(indices);
		return _scene.addEntity(e);
	}

	void Renderer::renderScene()
	{
		_scene.render();
	}

}
