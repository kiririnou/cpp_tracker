#ifndef ACTIVE_WINDOW_H
#define ACTIVE_WINDOW_H

#include <string>
#include <string_view>
#include <memory>

#include "rapidjson/prettywriter.h"

class ActiveWindow
{
public:
    ActiveWindow() {}

    ActiveWindow(std::string_view title, std::string_view timeStart)
        : m_title(title), m_timeStart(timeStart) {}

    ActiveWindow(std::string_view title, std::string_view timeStart, std::string_view timeEnd)
        : m_title(title), m_timeStart(timeStart), m_timeEnd(timeEnd) {}

    ~ActiveWindow() {}

    ActiveWindow(const ActiveWindow& aw)
    {
        m_title = aw.m_title;
        m_timeStart = aw.m_timeStart;
        m_timeEnd = aw.m_timeEnd;
    }

    ActiveWindow(ActiveWindow&& aw) = delete;

    ActiveWindow& operator=(const ActiveWindow& other)
    {
        if (this == &other)
            return *this;

        m_title = other.m_title;
        m_timeStart = other.m_timeStart;
        m_timeEnd = other.m_timeEnd;
        return *this;
    }

    std::unique_ptr<ActiveWindow> Copy()
    {
        return std::make_unique<ActiveWindow>(m_title, m_timeStart, m_timeEnd);
    }

    std::string GetTitle() const
    {
        return m_title;
    }

    std::string GetStartTime() const
    {
        return m_timeStart;
    }

    void SetEndTime(std::string_view endTime)
    {
        m_timeEnd = endTime;
    }

    template <typename Writer>
    void Serialize(Writer& writer)
    {
        writer.String("title");
        writer.String(m_title.c_str(), static_cast<rapidjson::SizeType>(m_title.length()));
        writer.String("start");
        writer.String(m_timeStart.c_str(), static_cast<rapidjson::SizeType>(m_timeStart.length()));
        writer.String("end");
        writer.String(m_timeEnd.c_str(), static_cast<rapidjson::SizeType>(m_timeEnd.length()));
    }

private:
    std::string m_title;
    std::string m_timeStart;
    std::string m_timeEnd;
};

#endif