#include "lhpch.h"

#include "../Log.h"
#include "Renderer.h"
#include "Camera.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lighthouse
{

	static Scene _scene;
	static std::unique_ptr<Shader> _shader;
	static unsigned int _width;
	static unsigned int _height;
	static Camera _camera;
	static glm::vec3 _lightPosition;
	static glm::vec4 _lightColor;

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

		_shader = std::make_unique<Shader>(ShaderType::TEXTURE);
		_shader->bind();

		_camera = Camera(_width, _height);

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

		_camera.setProjection();
	}

	void Renderer::setShaderModel(glm::mat4 model)
	{
		_shader->setUniformMat4f("u_model", model);
	}

	Camera& Renderer::getCamera()
	{
		return _camera;
	}

	glm::vec3 Renderer::getLightPosition()
	{
		return _lightPosition;
	}

	void Renderer::setLightPosition(glm::vec3 lightPosition)
	{
		_lightPosition = lightPosition;
	}
	
	void Renderer::setLightColor(glm::vec4 lightColor)
	{
		_lightColor = lightColor;
	}

	void Renderer::setLightUniforms()
	{
		_shader->setUniform4f("u_lightColor", _lightColor[0], _lightColor[1], _lightColor[2], _lightColor[3]);
		_shader->setUniform3f("u_lightPosition", _lightPosition[0], _lightPosition[1], _lightPosition[2]);
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
		std::vector<std::array<float, 3>> allVertexNormals;

		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::map<std::array<float, 8>, unsigned int> vertexRegistry;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, sizeof(line));

			std::istringstream ss(line);

			std::string label;
			ss >> label;

			if (label == "v")
			{
				std::array<float, 3> vertexCoord;
				ss >> vertexCoord[0] >> vertexCoord[1] >> vertexCoord[2];
				allVerticesCoord.push_back(vertexCoord);
			}

			if (label == "vn")
			{
				std::array<float, 3> vertexNormal;
				ss >> vertexNormal[0] >> vertexNormal[1] >> vertexNormal[2];
				allVertexNormals.push_back(vertexNormal);
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
					size_t begin = 0;
					size_t end = strVertex[i].find("/");
					int vertexCoordIndex = stoi(strVertex[i].substr(begin, end));
					begin = end + 1;
					end = strVertex[i].find("/", begin);
					int textureCoordIndex = stoi(strVertex[i].substr(begin, end ));
					begin = end + 1;
					int vertexNormalIndex = stoi(strVertex[i].substr(begin));
					std::array<float, 3> vertexCoord = allVerticesCoord[vertexCoordIndex - 1];
					std::array<float, 2> textureCoord = allTextureCoord[textureCoordIndex - 1];
					std::array<float, 3> vertexNormal = allVertexNormals[vertexNormalIndex - 1];
					std::array<float, 8> vertex { 
						vertexCoord[0], vertexCoord[1], vertexCoord[2], 
						textureCoord[0], textureCoord[1], 
						vertexNormal[0], vertexNormal[1],vertexNormal[2],   
					};
					if (vertexRegistry.find(vertex) != vertexRegistry.end())
					{
						indices.push_back(vertexRegistry[vertex]);
					}
					else
					{
						vertexRegistry.insert(std::make_pair<>(vertex, vertices.size() / 8));
						indices.push_back(static_cast<int>(vertices.size()) / 8);
						for (int j = 0; j < 8; j++)
						{
							vertices.push_back(vertex[j]);
						}
					}
				}
			}
		}

		LH_CORE_INFO("Total vertices: {0} ", vertices.size() / 8);
		return Renderer::addEntity(name, vertices, indices, ShaderType::TEXTURE);
	}

	void Renderer::renderScene()
	{
		_scene.render();
	}

}
