#include "TestApp.h"

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
	Lighthouse::Renderer::renderScene();

}

void TestApp::onEvent(Lighthouse::Event& e)
{
}

void TestApp::_buildScene()
{
	Lighthouse::Renderer::addEntity(
		"TRIANGLE_1", 
		{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.0f,  0.5f
		},
		{
			0, 1, 2
		}
	);

	Lighthouse::Renderer::addEntity(
		"SQUARE_1",
		{
			-0.9f,  0.7f,
			-0.7f,  0.7f,
			-0.7f,  0.9f,
			-0.9f,  0.9f
		},
		{
			0, 1, 2,
			0, 2, 3
		}
	);

}
