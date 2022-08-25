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

	// to be defined in client
	Application* createApplication();

}
