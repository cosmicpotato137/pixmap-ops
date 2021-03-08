#include "Log.h"
#include <iostream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "GLFW/glfw3.h"


std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init(const std::string& name = "PROJECT")
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_CoreLogger = spdlog::stdout_color_mt(name);
    s_CoreLogger->set_level(spdlog::level::trace);

    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);
}

void GLClearError()
{
    while (glGetError())
    {
        std::cout << "Clearing Errors" << std::endl;
    }
}

bool GLLogCall(const char* function, const char* file, unsigned int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") in "
            << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
