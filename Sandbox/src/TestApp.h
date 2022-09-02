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
	Lighthouse::Scene _scene;

	void _buildScene();
	void _moveSideways(Lighthouse::Entity* e);
	std::vector<Lighthouse::Entity*> _entities;
};

