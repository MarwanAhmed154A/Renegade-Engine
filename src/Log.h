#pragma once

#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace RG
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_coreLogger; }
		static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

#ifdef _DEBUG
#define RG_CORE_INFO(...) RG::Log::GetCoreLogger()->info(__VA_ARGS__);
#define RG_CORE_WARN(...) RG::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define RG_CORE_ERROR(...) RG::Log::GetCoreLogger()->error(__VA_ARGS__);

#define RG_CLIENT_INFO(...) RG::Log::GetClientLogger()->info(__VA_ARGS__);
#define RG_CLIENT_WARN(...) RG::Log::GetClientLogger()->warn(__VA_ARGS__);
#define RG_CLIENT_ERROR(...) RG::Log::GetClientLogger()->error(__VA_ARGS__);
#else
#define RG_CORE_INFO(x) ;
#define RG_CORE_WARN(x) ;
#define RG_CORE_ERROR(x) ;

#define RG_CLIENT_INFO(x) ;
#define RG_CLIENT_WARN(x) ;
#define RG_CLIENT_ERROR(x) ;
#endif