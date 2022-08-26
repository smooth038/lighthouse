#pragma once

#include <Lighthouse.h>
#include <Lighthouse/Entrypoint.h>

#include "TestApp.h"

class Sandbox : public Lighthouse::Application
{
public:
	Sandbox()
	{
		Lighthouse::Application::pushLayer(new TestApp());
	}

	~Sandbox()
	{
	}
};

Lighthouse::Application* Lighthouse::createApplication()
{
	return new Sandbox();
}