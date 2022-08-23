#include <Lighthouse.h>

class Sandbox : public Lighthouse::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};

Lighthouse::Application* Lighthouse::createApplication()
{
	return new Sandbox();
}