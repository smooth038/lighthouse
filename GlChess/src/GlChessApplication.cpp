#pragma once

#include <Lighthouse.h>
#include <Lighthouse/Entrypoint.h>

#include "GlChess.h"

class GlChessApplication : public Lighthouse::Application
{
public:
	GlChessApplication() : Lighthouse::Application(1280, 720, "GlChess3D")
	{
		_app = std::make_shared<GlChess>(_window);
		Lighthouse::Application::pushLayer(_app);
	}

	~GlChessApplication()
	{
	}

private:
	std::shared_ptr<Lighthouse::Layer> _app;
};

std::unique_ptr<Lighthouse::Application> Lighthouse::createApplication()
{
	return std::make_unique<GlChessApplication>();
}