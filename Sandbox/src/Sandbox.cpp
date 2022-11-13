#pragma once

#include <Lighthouse.h>
#include <Lighthouse/Entrypoint.h>

#include "TestApp.h"

class Sandbox : public Lighthouse::Application
{
public:
	Sandbox()
	{
		_app = std::make_shared<TestApp>(_window);
		Lighthouse::Application::pushLayer(_app);
	}

	~Sandbox()
	{
	}

private:
	std::shared_ptr<Lighthouse::Layer> _app;
};

std::shared_ptr<Lighthouse::Application> Lighthouse::createApplication()
{
	return std::make_unique<Sandbox>();
}