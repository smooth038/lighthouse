#pragma once

#include <Lighthouse.h>
#include <Lighthouse/Entrypoint.h>

#include "TestApp.h"

class Sandbox : public Lighthouse::Application
{
public:
	Sandbox()
	{
		_app = std::make_unique<TestApp>(_window);
		Lighthouse::Application::pushLayer(_app);
	}

	~Sandbox()
	{
	}

private:
	std::unique_ptr<Lighthouse::Layer> _app;
};

std::unique_ptr<Lighthouse::Application> Lighthouse::createApplication()
{
	return std::make_unique<Sandbox>();
}