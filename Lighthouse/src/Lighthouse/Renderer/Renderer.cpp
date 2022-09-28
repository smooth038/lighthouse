#include "lhpch.h"

#include "../Log.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lighthouse
{

	static Scene _scene;
	static std::unique_ptr<Shader> _shader;
	static unsigned int _width;
	static unsigned int _height;
	static glm::mat4 _matProj;

	void Renderer::init(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;

		glViewport(0, 0, width, height);

		glewExperimental = GL_TRUE;
		glewInit();

		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		unsigned int ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		_shader = std::make_unique<Shader>(ShaderType::FLAT_COLOR);
		_shader->bind();

		computeProjectionMatrix();

		glEnable(GL_DEPTH_TEST);
	}

	std::unique_ptr<Shader>& Renderer::getShader()
	{
		return _shader;
	}

	void Renderer::setShaderType(ShaderType type)
	{
		_shader = std::make_unique<Shader>(type);
		_shader->bind();

		computeProjectionMatrix();
	}

	void Renderer::setShaderModel(glm::mat4 model)
	{
		_shader->setUniformMat4f("u_model", model);
	}

	void Renderer::setShaderView(glm::mat4 view)
	{
		_shader->setUniformMat4f("u_view", view);
	}

	void Renderer::computeProjectionMatrix()
	{
		float w = static_cast<float>(_width);
		float h = static_cast<float>(_height);
		float fov = 90.0f;
		float zFar = 100.0f;
		float zNear = 0.1f;
		float aspectRatio = w / h;

		_matProj = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
		_shader->setUniformMat4f("u_proj", _matProj);
	}

	void Renderer::setWindowSize(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;
		glViewport(0, 0, width, height);
	}

	Scene& Renderer::getScene()
	{
		return _scene;
	}

	std::unique_ptr<Entity>& Renderer::addEntity(const std::string id, std::vector<float> vertices, std::vector<unsigned int> indices, ShaderType shaderType)
	{
		Entity e(id);
		e.addVertices(vertices);
		e.addIndices(indices);
		e.setShaderType(shaderType);
		return _scene.addEntity(e);
	}

	std::unique_ptr<Entity>& Renderer::loadObjFile(const std::string& filepath, const std::string& name)
	{
		std::ifstream f(filepath);
		if (!f.is_open())
		{
			LH_FATAL("Could not open mesh file!");
			throw "Error";
		}

		std::vector<std::array<float, 3>> allVerticesCoord;
		std::vector<std::array<float, 2>> allTextureCoord;

		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::map<std::array<float, 5>, unsigned int> vertexRegistry;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::istringstream ss(line);

			std::string label;
			ss >> label;

			if (label == "v")
			{
				std::array<float, 3> vertexCoord;
				ss >> vertexCoord[0] >> vertexCoord[1] >> vertexCoord[2];
				allVerticesCoord.push_back(vertexCoord);
			}

			if (label == "vt")
			{
				std::array<float, 2>textureCoord;
				ss >> textureCoord[0] >> textureCoord[1];
				allTextureCoord.push_back(textureCoord);
			}

			if (label == "f")
			{
				std::vector<std::string> strVertex(3);
				ss >> strVertex[0] >> strVertex[1] >> strVertex[2];
				for (int i = 0; i < 3; i++)
				{
					int vertexCoordIndex = stoi(strVertex[i].substr(0, strVertex[i].find("/")));
					int textureCoordIndex = stoi(strVertex[i].substr(strVertex[i].find("/") + 1));
					std::array<float, 3> vertexCoord = allVerticesCoord[vertexCoordIndex - 1];
					std::array<float, 2> textureCoord = allTextureCoord[textureCoordIndex - 1];
					std::array<float, 5> vertex { vertexCoord[0], vertexCoord[1], vertexCoord[2], textureCoord[0], textureCoord[1]};
					if (vertexRegistry.find(vertex) != vertexRegistry.end())
					{
						indices.push_back(vertexRegistry[vertex]);
					}
					else
					{
						vertexRegistry.insert(std::make_pair<>(vertex, vertices.size() / 5));
						indices.push_back(static_cast<int>(vertices.size()) / 5);
						for (int j = 0; j < 5; j++)
						{
							vertices.push_back(vertex[j]);
						}
					}
				}
			}
		}

		LH_CORE_INFO("Total vertices: {0} ", vertices.size() / 5);
		return Renderer::addEntity(name, vertices, indices, ShaderType::TEXTURE);
	}

	void Renderer::renderScene()
	{
		_scene.render();
	}

}
