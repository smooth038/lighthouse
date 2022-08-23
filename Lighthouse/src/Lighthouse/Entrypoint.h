#pragma once

#ifdef LH_PLATFORM_WINDOWS

extern Lighthouse::Application* Lighthouse::createApplication();

int main(int argc, char** argv)
{
	auto app = Lighthouse::createApplication();
	app->run();
	delete app;
}

#endif
