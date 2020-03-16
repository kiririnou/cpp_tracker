#include "logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> logger::s_UsrLogger;
std::shared_ptr<spdlog::logger> logger::s_WndLogger;
std::shared_ptr<spdlog::logger> logger::s_ResLogger;

void logger::Init()	
{
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] <%n> [td_id: %t] [%^%l%$]: %v");
		s_UsrLogger = spdlog::stdout_color_mt("UserCheck");
		s_UsrLogger->set_level(spdlog::level::trace);

		s_WndLogger = spdlog::stdout_color_mt("WndActivity");
		s_WndLogger->set_level(spdlog::level::trace);

		s_ResLogger = spdlog::stdout_color_mt("ResActivity");
		s_ResLogger->set_level(spdlog::level::trace);
}
