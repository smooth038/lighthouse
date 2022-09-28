#pragma once

#ifdef LH_PLATFORM_WINDOWS

extern std::unique_ptr<Lighthouse::Application> Lighthouse::createApplication();

int main(int argc, char** argv)
{
	Lighthouse::Log::init();
	LH_CORE_WARN("Initialized log!");
	int a = 5;
	LH_INFO("Hello! Var={0}", a);

	auto app = Lighthouse::createApplication();
	app->run();
}

#endif
