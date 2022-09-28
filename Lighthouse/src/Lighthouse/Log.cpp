#include "lhpch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_sinks.h>

namespace Lighthouse
{

	std::shared_ptr<spdlog::logger> Log::_coreLogger;
	std::shared_ptr<spdlog::logger> Log::_clientLogger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n : %v%$");

		_coreLogger = spdlog::stdout_color_mt("LH");
		_coreLogger->set_level(spdlog::level::trace);

		_clientLogger = spdlog::stdout_color_mt("APP");
		_clientLogger->set_level(spdlog::level::trace); 
	}

	std::shared_ptr<spdlog::logger>& Log::getCoreLogger()
	{
		return _coreLogger;
	}

	std::shared_ptr<spdlog::logger>& Log::getClientLogger()
	{
		return _clientLogger;
	}

}
