#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Lighthouse
{

	class LH_API Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return _coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return _clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> _coreLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;
	};

}

// Core log macros
#define LH_CORE_TRACE(...) ::Lighthouse::Log::getCoreLogger()->trace(__VA_ARGS__)
#define LH_CORE_INFO(...)  ::Lighthouse::Log::getCoreLogger()->info(__VA_ARGS__)
#define LH_CORE_WARN(...)  ::Lighthouse::Log::getCoreLogger()->warn(__VA_ARGS__)
#define LH_CORE_ERROR(...) ::Lighthouse::Log::getCoreLogger()->error(__VA_ARGS__)
#define LH_CORE_FATAL(...) ::Lighthouse::Log::getCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define LH_TRACE(...) ::Lighthouse::Log::getClientLogger()->trace(__VA_ARGS__)
#define LH_INFO(...)  ::Lighthouse::Log::getClientLogger()->info(__VA_ARGS__)
#define LH_WARN(...)  ::Lighthouse::Log::getClientLogger()->warn(__VA_ARGS__)
#define LH_ERROR(...) ::Lighthouse::Log::getClientLogger()->error(__VA_ARGS__)
#define LH_FATAL(...) ::Lighthouse::Log::getClientLogger()->fatal(__VA_ARGS__)
