#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Axel {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};
}

#define AX_TRACE(...)		::Axel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AX_INFO(...)		::Axel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AX_WARN(...)		::Axel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AX_ERROR(...)		::Axel::Log::GetCoreLogger()->error(__VA_ARGS__)