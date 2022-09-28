#include "TestApp.h"
#include <glm/gtc/type_ptr.hpp>

void TestApp::onAttach()
{
	LH_INFO("TestApp attached!");

	_buildScene();
}

void TestApp::onDetach()
{
	LH_INFO("TestApp detached!");
}

void TestApp::onUpdate()
{
	Lighthouse::RenderCommand::fillCanvas(0.03125f, 0.0546875f, 0.25f, 1.0f);
	_rotate(Lighthouse::Renderer::getScene().getEntityById("king"), 0.5f, glm::vec3(0, 1, 0));
	Lighthouse::Renderer::renderScene();
}

void TestApp::onEvent(Lighthouse::Event&)
{
}

void TestApp::_buildScene()
{
	std::unique_ptr<Lighthouse::Entity>& king = _addEntityFromFile("res\\meshes\\king.obj", "king");
	Lighthouse::Texture texture("res\\textures\\wood.jpg");
	texture.bind(1);
	king->setTextureSlot(1);
	_translate(king, glm::vec3(0.0f, -5.0f, -10.0f));
}

void TestApp::_rotate(std::unique_ptr<Lighthouse::Entity>& e, float deg, glm::vec3 axis)
{
	glm::mat4 model = e->getModelMatrix();
	model = glm::rotate(model, glm::radians(deg), axis);
	e->setModelMatrix(model);
}

void TestApp::_translate(std::unique_ptr<Lighthouse::Entity>& e, glm::vec3 translationVector)
{
	glm::mat4 translate(1.0f);
	translate = glm::translate(translate, translationVector);
	e->setViewMatrix(translate);
}

std::unique_ptr<Lighthouse::Entity>& TestApp::_addEntityFromFile(const std::string& filepath, const std::string& name)
{
	_entityIds.push_back(name);
	Lighthouse::Renderer::loadObjFile(filepath, name);
	return Lighthouse::Renderer::getScene().getEntityById(_entityIds.back());
}

