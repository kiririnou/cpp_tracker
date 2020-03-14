#ifndef LOGGER_H
#define LOGGER_H

#include <string_view>
#include <mutex>
#include <windows.h>
#include "utils.h"
#include "system_info.h"

std::mutex mtx;

class logger
{
public:
    logger(std::string_view name)
    {
        m_name = name;
        m_hInstance = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    void log(std::string_view data)
    {
        std::lock_guard<std::mutex> guard(mtx);
        std::clog << "[" << system_info::GetNow() << "] <" << m_name << "> " << data << std::endl;
    }

    void info(std::string_view data)
    {
        std::lock_guard<std::mutex> guard(mtx);
        std::clog << "[" << system_info::GetNow() << "] <" << m_name << "> [";
        SetConsoleTextAttribute(m_hInstance, 10);
        std::clog << "info";
        SetConsoleTextAttribute(m_hInstance, 7);
        std::clog << "] " << data << std::endl;
    }

    void warn(std::string_view data)
    {
        std::lock_guard<std::mutex> guard(mtx);
        std::clog << "[" << system_info::GetNow() << "] <" << m_name << "> [";
        SetConsoleTextAttribute(m_hInstance, 14);
        std::clog << "warn";
        SetConsoleTextAttribute(m_hInstance, 7);
        std::clog << "] " << data << std::endl;
    }

    void error(std::string_view data)
    {
        std::lock_guard<std::mutex> guard(mtx);
        std::clog << "[" << system_info::GetNow() << "] <" << m_name << "> [";
        SetConsoleTextAttribute(m_hInstance, 12);
        std::clog << "error";
        SetConsoleTextAttribute(m_hInstance, 7);
        std::clog << "] " << data << std::endl;
    }

    void critical(std::string_view data)
    {
        std::lock_guard<std::mutex> guard(mtx);
        std::clog << "[" << system_info::GetNow() << "] <" << m_name << "> [";
        SetConsoleTextAttribute(m_hInstance, 192);
        std::clog << "critical";
        SetConsoleTextAttribute(m_hInstance, 7);
        std::clog << "] " << data << std::endl;
    }

private:
    std::string_view m_name;
    HANDLE m_hInstance;
};

#endif