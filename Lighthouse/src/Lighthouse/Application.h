#pragma once

#include "Core.h"

namespace Lighthouse {

	class LH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	// To be defined in CLIENT
	Application* createApplication();

}
