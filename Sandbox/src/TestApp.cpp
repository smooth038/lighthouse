#include "TestApp.h"

void TestApp::onAttach()
{
	LH_INFO("TestApp attached!");
}

void TestApp::onDetach()
{
	LH_INFO("TestApp detached!");
}

void TestApp::onUpdate()
{
	Lighthouse::RenderCommand::fillCanvas(1, 0, 1, 1);
}

void TestApp::onEvent(Lighthouse::Event& e)
{
}
