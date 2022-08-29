#include "TestApp.h"

void TestApp::onAttach()
{
	LH_INFO("TestApp attached!");
}

void TestApp::onDetach()
{
	LH_INFO("TestApp detached!");
}

void TestApp::onUpdate(Lighthouse::Renderer* renderer)
{
	Lighthouse::RenderCommand::fillCanvas(1, 0, 1, 1);

	renderer->testDrawTriangle();

}

void TestApp::onEvent(Lighthouse::Event& e)
{
}
