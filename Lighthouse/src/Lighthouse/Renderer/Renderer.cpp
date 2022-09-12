#include "lhpch.h"

#include "Renderer.h"
#include "GL/glew.h"
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
		float w = _width;
		float h = _height;
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

	Entity* Renderer::addEntity(std::string id, std::vector<float> vertices, std::vector<unsigned int> indices, ShaderType shaderType)
	{
		Entity e(id);
		e.addVertices(vertices);
		e.addIndices(indices);
		e.setShaderType(shaderType);
		return _scene.addEntity(e);
	}

	void Renderer::renderScene()
	{
		_scene.render();
	}

}
