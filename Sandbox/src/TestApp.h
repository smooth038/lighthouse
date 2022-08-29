#pragma once

#include "Lighthouse.h"

class TestApp : public Lighthouse::Layer
{
public:
	TestApp() : Lighthouse::Layer("Test app!") {}
	virtual ~TestApp() {}

	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onUpdate(Lighthouse::Renderer* renderer) override;
	virtual void onEvent(Lighthouse::Event& e) override;

};

