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
	void _moveSideways(std::unique_ptr<Lighthouse::Entity>& e);
	void _rotate(std::unique_ptr<Lighthouse::Entity>& e, float deg, glm::vec3 axis);
	void _translate(std::unique_ptr<Lighthouse::Entity>& e, glm::vec3 translationVector);
	std::unique_ptr<Lighthouse::Entity>& _addEntityFromFile(const std::string& filepath, const std::string& name);
	std::vector<std::string> _entityIds;
};

