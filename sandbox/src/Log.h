#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


class Log
{
public:
	static void Init(const std::string& name);

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// clears all active gl errors
void GLClearError();

// calls glGetError() to find new errors
bool GLLogCall(const char* function, const char* file, unsigned int line);

#ifndef DIST
// core Log macros
#define CORE_TRACE(...)		::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)		::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)		::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...)		::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...)	::Log::GetCoreLogger()->critical(__VA_ARGS__)

// client Log macros
#define CLIENT_TRACE(...)		::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CLIENT_INFO(...)		::Log::GetClientLogger()->info(__VA_ARGS__)
#define CLIENT_WARN(...)		::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CLIENT_ERROR(...)		::Log::GetClientLogger()->error(__VA_ARGS__)
#define CLIENT_CRITICAL(...)	::Log::GetClientLogger()->critical(__VA_ARGS__)

#else
#define CORE_TRACE(...)
#define CORE_INFO(...)
#define CORE_WARN(...)
#define CORE_ERROR(...)
#define CORE_CRITICAL(...)

#define CLIENT_TRACE(...)	
#define CLIENT_INFO(...)	
#define CLIENT_WARN(...)	
#define CLIENT_ERROR(...)	
#define CLIENT_CRITICAL(...)
#endif


// debug assertions
#ifdef ENABLE_ASSERTS
#define CLIENT_ASSERT(x, ...) {if(!(x)) { CLIENT_ERROR(__VA_ARGS__); __debugbreak(); } }
#define CORE_ASSERT(x, ...) {if(!(x)) { CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
#else
#define CLIENT_ASSERT(x, ...)
#define CORE_ASSERT(x, ...)
#endif

// debug validations
#define CLIENT_VALIDATE(x, y, ...) { if (!(x)) { CLIENT_WARN(__VA_ARGS__); y; } }
#define CORE_VALIDATE(x, y, ...) { if(!(x)) { CORE_WARN(__VA_ARGS__); y; } }