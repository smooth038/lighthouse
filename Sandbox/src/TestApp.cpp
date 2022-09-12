#include "TestApp.h"
#include <glm/gtc/type_ptr.hpp>

void TestApp::onAttach()
{
	LH_INFO("TestApp attached!");

	_buildScene();
	_translate(_entities[0], 0.0f, -0.1f, -1.0f);
}

void TestApp::onDetach()
{
	LH_INFO("TestApp detached!");
}

void TestApp::onUpdate()
{
	Lighthouse::RenderCommand::fillCanvas(0.03125f, 0.0546875f, 0.25f, 1.0f);
	_moveSideways(_entities[0]);
	_rotate(_entities[0], 0.5f, glm::vec3(0, 1, 1));
	Lighthouse::Renderer::renderScene();

}

void TestApp::onEvent(Lighthouse::Event& e)
{
}

void TestApp::_buildScene()
{
	_entities.push_back(Lighthouse::Renderer::addEntity(
		"CUBE",
		{
			-0.1f, -0.1f, -0.1f, 0.5f, 0.5f, 0.0f, 1.0f,
			 0.1f, -0.1f, -0.1f, 0.5f, 0.5f, 0.0f, 1.0f,
			 0.1f,  0.1f, -0.1f, 0.5f, 0.5f, 0.0f, 1.0f,
			-0.1f,  0.1f, -0.1f, 0.5f, 0.5f, 0.0f, 1.0f,
			-0.1f, -0.1f,  0.1f, 0.0f, 0.0f, 0.5f, 1.0f,
			 0.1f, -0.1f,  0.1f, 0.0f, 0.0f, 0.5f, 1.0f,
			 0.1f,  0.1f,  0.1f, 0.0f, 0.0f, 0.5f, 1.0f,
			-0.1f,  0.1f,  0.1f, 0.0f, 0.0f, 0.5f, 1.0f,
		},
		{
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			3, 2, 6,
			3, 6, 7,
			0, 5, 1,
			0, 4, 5,
			0, 3, 7,
			0, 7, 4,
			1, 5, 6,
			1, 6, 2,
		}
	));

}

void TestApp::_moveSideways(Lighthouse::Entity* e)
{
	static std::unordered_map<Lighthouse::Entity*, bool> movingRightMap;

	if (!movingRightMap.count(e)) movingRightMap.insert(std::make_pair(e, true));

	float xMin = -0.8f;
	float xMax = 0.8f;

	glm::mat4 view = e->getViewMatrix();
	float x = view[3][0];

	if (x + 0.01f > xMax && movingRightMap[e]) movingRightMap[e] = false;
	if (x - 0.01f < xMin && !movingRightMap[e]) movingRightMap[e] = true;

	movingRightMap[e] ? view[3][0] += 0.01f : view[3][0] -= 0.01f;

	e->setViewMatrix(view);
}

void TestApp::_rotate(Lighthouse::Entity* e, float deg, glm::vec3 axis)
{
	glm::mat4 model = e->getModelMatrix();
	model = glm::rotate(model, glm::radians(deg), axis);
	e->setModelMatrix(model);
}

void TestApp::_translate(Lighthouse::Entity* e, float x, float y, float z)
{
	glm::mat4 translate(1.0f);
	translate = glm::translate(translate, glm::vec3(x, y, z));
	e->setViewMatrix(translate);
}

