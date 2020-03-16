#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include "spdlog/spdlog.h"

class logger
{
public:
	static void Init();
	
	inline static std::shared_ptr<spdlog::logger>& GetUsrLogger() { return s_UsrLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetWndLogger() { return s_WndLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetResLogger() { return s_ResLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_UsrLogger;
	static std::shared_ptr<spdlog::logger> s_WndLogger;
	static std::shared_ptr<spdlog::logger> s_ResLogger;
};

#define WND_TRACE(...)	::logger::GetWndLogger()->trace(__VA_ARGS__)
#define WND_INFO(...)	::logger::GetWndLogger()->info(__VA_ARGS__)
#define WND_WARN(...)	::logger::GetWndLogger()->warn(__VA_ARGS__)
#define WND_ERROR(...)	::logger::GetWndLogger()->error(__VA_ARGS__)
#define WND_FATAL(...)	::logger::GetWndLogger()->critical(__VA_ARGS__)

#define RES_TRACE(...)	::logger::GetResLogger()->trace(__VA_ARGS__)
#define RES_INFO(...)	::logger::GetResLogger()->info(__VA_ARGS__)
#define RES_WARN(...)	::logger::GetResLogger()->warn(__VA_ARGS__)
#define RES_ERROR(...)	::logger::GetResLogger()->error(__VA_ARGS__)
#define RES_FATAL(...)	::logger::GetResLogger()->critical(__VA_ARGS__)

#define USR_TRACE(...)	::logger::GetUsrLogger()->trace(__VA_ARGS__)
#define USR_INFO(...)	::logger::GetUsrLogger()->info(__VA_ARGS__)
#define USR_WARN(...)	::logger::GetUsrLogger()->warn(__VA_ARGS__)
#define USR_ERROR(...)	::logger::GetUsrLogger()->error(__VA_ARGS__)
#define USR_FATAL(...) 	::logger::GetUsrLogger()->critical(__VA_ARGS__)

#endif
