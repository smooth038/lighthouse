#pragma once

#include "Lighthouse.h"

class Lighthouse::Scene;

class TestApp : public Lighthouse::Layer
{
public:
	TestApp() : Lighthouse::Layer("Test app!") {}
	virtual ~TestApp() {}

	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onUpdate() override;
	virtual void onEvent(Lighthouse::Event& e) override;

private:
	void _buildScene();
	void _rotateEntity(std::unique_ptr<Lighthouse::Entity>& e, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition);
	void _translateEntity(std::unique_ptr<Lighthouse::Entity>& e, glm::vec3 translationVector);
	glm::mat4 _rotateMatrix(glm::mat4 m, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition);
	glm::vec3 _rotateVector(glm::vec3 v, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition);
	glm::mat4 _translateMatrix(glm::mat4 m, glm::vec3 translation);
	glm::vec3 _translateVector(glm::vec3 m, glm::vec3 translation);
	std::unique_ptr<Lighthouse::Entity>& _addEntityFromFile(const std::string& filepath, const std::string& name);
	std::unique_ptr<Lighthouse::Entity>& _addCube(const std::string& name, glm::vec3 position, glm::vec4 color, float edgeSize);
	std::vector<std::string> _entityIds;
};

