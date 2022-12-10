#include "lhpch.h"

#include "../Log.h"
#include "Renderer.h"
#include "Camera.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lighthouse
{
	static unsigned int _width;
	static unsigned int _height;

	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ibo;

	unsigned int _renderFbo;
	unsigned int _renderTexture;
	unsigned int _renderDepthTexture;
	unsigned int _pickingFbo;
	unsigned int _pickingTexture;
	unsigned int _pickingDepthTexture;

	static Scene _scene;
	static std::unique_ptr<Shader> _shader;
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

		//glGenVertexArrays(1, &_vao);
		//glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glGenBuffers(1, &_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

		_shader = std::make_unique<Shader>(ShaderType::TEXTURE);
		_shader->bind();

		_camera = Camera(_width, _height);

		glEnable(GL_DEPTH_TEST);

		initRenderFrameBuffer();
		initPickingFrameBuffer();
	}

	void Renderer::initRenderFrameBuffer()
	{
		glGenFramebuffers(1, &_renderFbo);
		glGenTextures(1, &_renderTexture);
		glGenTextures(1, &_renderDepthTexture);

		updateRenderFrameBuffer();
	}

	void Renderer::updateRenderFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _renderFbo);

		// we use texture slot 28 and 29 for render and render depth texture
		glActiveTexture(GL_TEXTURE28);
		glBindTexture(GL_TEXTURE_2D, _renderTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTexture, 0);

		glActiveTexture(GL_TEXTURE29);
		glBindTexture(GL_TEXTURE_2D, _renderDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _renderDepthTexture, 0);


		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			LH_CORE_ERROR("Error while binding render frame buffer. {0}", status);
			throw;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	unsigned int Renderer::getRenderFbo()
	{
		return _renderFbo;
	}

	unsigned int Renderer::getRenderTexture()
	{
		return _renderTexture;
	}

	void Renderer::initPickingFrameBuffer()
	{
		glGenFramebuffers(1, &_pickingFbo);
		glGenTextures(1, &_pickingTexture);
		glGenTextures(1, &_pickingDepthTexture);

		updatePickingFrameBuffer(false);
	}

	void Renderer::updatePickingFrameBuffer(bool rerender)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _pickingFbo);

		// we use texture slot 30 and 31 for picking and depth texture
		glActiveTexture(GL_TEXTURE30);
		glBindTexture(GL_TEXTURE_2D, _pickingTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pickingTexture, 0);

		glActiveTexture(GL_TEXTURE31);
		glBindTexture(GL_TEXTURE_2D, _pickingDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _pickingDepthTexture, 0);


		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			LH_CORE_ERROR("Error while binding picking frame buffer. {0}", status);
			throw;
		}

		if (rerender)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _pickingFbo);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			_scene.render(true);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		e.addShaderType(shaderType);
		return _scene.addEntity(e);
	}

	std::unique_ptr<Entity>& Renderer::loadLvfFile(const std::string& lvfPath, const std::string& name)
	{
		std::ifstream lvf(lvfPath, std::ios::in | std::ios::binary);
		if (!lvf.is_open())
		{
			LH_FATAL("Could not open LVF file!");
			throw "Error";
		}

		size_t numberOfVertices;
		size_t numberOfIndices;

		lvf.read((char*)&numberOfVertices, sizeof(size_t));
		lvf.read((char*)&numberOfIndices, sizeof(size_t));

		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		for (int i = 0; i < numberOfVertices; i += 8)
		{
			std::array<float, 8> v{};
			lvf.read((char*) v.data(), 8 * sizeof(float));
			vertices.insert(vertices.end(), std::begin(v), std::end(v));
		}
		for (int i = 0; i < numberOfIndices; i += 3)
		{
			std::array<unsigned int, 3> f{};
			lvf.read((char*) f.data(), 3 * sizeof(unsigned int));
			indices.insert(indices.end(), std::begin(f), std::end(f));
		}

		lvf.close();

		LH_CORE_INFO("Done processing {0} -> total vertices: {1} ", lvfPath, vertices.size() / 8);
		return Renderer::addEntity(name, vertices, indices, ShaderType::TEXTURE);
	}

	std::unique_ptr<Entity>& Renderer::loadObjFile(const std::string& filepath, const std::string& name)
	{
		if (filepath.substr(filepath.length() - 4, 4) != ".obj")
		{
			LH_CORE_ERROR("Cannot load obj file because the file {0} does not end with extension '.obj'.", filepath);
			throw;
		}

		// If an LVF file already exists, do not load OBJ file and load LVF file instead
		std::string lvfPath = filepath.substr(0, filepath.length() - 4) + ".lvf";
		std::ifstream lvf(lvfPath);
		if (lvf.good())
		{
			lvf.close();
			return loadLvfFile(lvfPath, name);
		}

		// Read OBJ file
		std::ifstream f(filepath);
		if (!f.is_open())
		{
			LH_FATAL("Could not open mesh file!");
			throw "Error";
		}

		std::vector<std::array<float, 3>> allVerticesCoord;
		std::vector<std::array<float, 3>> allVertexNormals;
		std::vector<std::array<float, 2>> allTextureCoord;

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
				std::array<float, 3> vertexCoord{};
				ss >> vertexCoord[0] >> vertexCoord[1] >> vertexCoord[2];
				allVerticesCoord.push_back(vertexCoord);
			}

			if (label == "vn")
			{
				std::array<float, 3> vertexNormal{};
				ss >> vertexNormal[0] >> vertexNormal[1] >> vertexNormal[2];
				allVertexNormals.push_back(vertexNormal);
			}

			if (label == "vt")
			{
				std::array<float, 2>textureCoord{};
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
					size_t vertexCoordIndex = stoi(strVertex[i].substr(begin, end));

					begin = end + 1;
					end = strVertex[i].find("/", begin);
					size_t textureCoordIndex = stoi(strVertex[i].substr(begin, end ));

					begin = end + 1;
					size_t vertexNormalIndex = stoi(strVertex[i].substr(begin));

					std::array<float, 3> vertexCoord = allVerticesCoord[vertexCoordIndex - 1];
					std::array<float, 2> textureCoord = allTextureCoord[textureCoordIndex - 1];
					std::array<float, 3> vertexNormal = allVertexNormals[vertexNormalIndex - 1];
					std::array<float, 8> vertex { 
						vertexCoord[0], vertexCoord[1], vertexCoord[2], 
						textureCoord[0], textureCoord[1], 
						vertexNormal[0], vertexNormal[1],vertexNormal[2],   
					};
					if (vertexRegistry.find(vertex) == vertexRegistry.end())
					{
						vertexRegistry.insert(std::make_pair<>(vertex, vertices.size() / 8));
						for (int j = 0; j < 8; j++)
						{
							vertices.push_back(vertex[j]);
						}
					}
					indices.push_back(vertexRegistry[vertex]);
				}
			}
		}

		f.close();

		// Now write LVF file and load it
		std::ofstream os(lvfPath, std::ios::out | std::ios::binary);
		if (!os.is_open())
		{
			LH_FATAL("Could not write LVF file!");
			throw "Error";
		}

		size_t numberOfVertices = vertices.size();
		size_t numberOfIndices = indices.size();
		os.write((char*)&numberOfVertices, sizeof(size_t));
		os.write((char*)&numberOfIndices, sizeof(size_t));

		for (int i = 0; i < numberOfVertices; i++)
		{
			os.write((char*) &vertices[i], sizeof(float));
		}
		for (int i = 0; i < numberOfIndices; i++)
		{
			os.write((char*) &indices[i], sizeof(unsigned int));
		}
		os.close();

		return loadLvfFile(lvfPath, name);
	}

	void Renderer::renderScene()
	{
		_scene.render();
	}

	void Renderer::onWindowResize(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;
		_camera.setDimensions(width, height);
		_camera.setProjection();
		glViewport(0, 0, width, height);
		updateRenderFrameBuffer();
		updatePickingFrameBuffer(true);
	}

	unsigned int Renderer::getObjectIndexFromPixel(unsigned int x, unsigned int y)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _pickingFbo);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		struct Pixel
		{
			unsigned char red, green, blue;
		};

		Pixel pixel;

		glReadPixels(x, _height - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
	
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (pixel.blue < 0 || pixel.blue > 255)
		{
			return -1;
		}

		return static_cast<unsigned int>(pixel.blue);
	}
}
