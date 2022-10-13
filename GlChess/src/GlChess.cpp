#pragma once

#include <Lighthouse.h>
#include <Lighthouse/Entrypoint.h>

#include "GlChess3D.h"

class GlChess : public Lighthouse::Application
{
public:
	GlChess() : Lighthouse::Application(1280, 720)
	{
		_app = std::make_unique<GlChess3D>(_window);
		Lighthouse::Application::pushLayer(_app);
	}

	~GlChess()
	{
	}

private:
	std::unique_ptr<Lighthouse::Layer> _app;
};

std::unique_ptr<Lighthouse::Application> Lighthouse::createApplication()
{
	return std::make_unique<GlChess>();
}