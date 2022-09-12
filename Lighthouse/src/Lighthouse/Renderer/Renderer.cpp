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

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 7 * sizeof(float), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, false, 7 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		unsigned int ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		_shader = std::make_unique<Shader>("src\\Lighthouse\\Shaders\\default.shader");
		_shader->bind();

		computeProjectionMatrix();

		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::computeProjectionMatrix()
	{
		float w = _width;
		float h = _height;
		float fov = glm::radians(90.0f);
		float zFar = 100.0f;
		float zNear = 0.1f;

		float a = w / h;
		float f = 1.0f / glm::tan(fov / 2.0f);
		float q = zFar / (zFar - zNear);

		//float projVals[16] = {
		//	a*f,  0.0f, 0.0f    , 0.0f,
		//	0.0f, f   , 0.0f    , 0.0f,
		//	0.0f, 0.0f, q       , 1.0f,
		//	0.0f, 0.0f, -zNear*q, 0.0f
		//};
		//_matProj = glm::make_mat4(projVals);
		_matProj = glm::perspective(fov, a, zNear, zFar);
		//for (int i = 0; i < 4; i++)
		//{
		//	for (int j = 0; j < 4; j++)
		//	{
		//		std::cout << _matProj[i][j] << " ";
		//	}
		//	std::cout << std::endl;
		//}

		_shader->setUniformMat4f("u_Proj", _matProj);
	}

	void Renderer::setWindowSize(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;
		glViewport(0, 0, width, height);
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
		_scene.render(_shader);
	}

}
