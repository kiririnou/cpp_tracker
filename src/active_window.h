#ifndef ACTIVE_WINDOW_H
#define ACTIVE_WINDOW_H

#include <string>
#include <string_view>

#include "rapidjson/prettywriter.h"

class ActiveWindow
{
public:
    ActiveWindow(std::string_view title, std::string_view timeStart, std::string_view timeEnd)
        : m_title(title), m_timeStart(timeStart), m_timeEnd(timeEnd) {}

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