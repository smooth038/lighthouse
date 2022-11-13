#pragma once

#ifdef LH_PLATFORM_WINDOWS

extern std::shared_ptr<Lighthouse::Application> Lighthouse::createApplication();

int main(int argc, char** argv)
{
	Lighthouse::Log::init();
	LH_CORE_WARN("Initialized log!");

	auto app = Lighthouse::createApplication();
	app->run();
}

#endif
