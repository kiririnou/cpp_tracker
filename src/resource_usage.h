#ifndef RESOURCE_USAGE_H
#define RESOURCE_USAGE_H

#include "rapidjson/prettywriter.h"
#include <string>

class Resource_Usage
{
public:
    Resource_Usage(const std::string& hwId, const std::string& start, const std::string& end, const std::string& processTitle, int mem)
        : m_hwId(hwId), m_start(start), m_end(end), m_processTitle(processTitle), m_mem(mem) 
    {} 
    ~Resource_Usage() {}

    void SetEndTime(const std::string& end)
    {
        m_end = end;
    }

    template <typename Writer>
    void Serialize(Writer& writer)
    {
        writer.String("hw_id");
        writer.String(m_hwId.c_str(), static_cast<rapidjson::SizeType>(m_hwId.length()));
        writer.String("start");
        writer.String(m_start.c_str(), static_cast<rapidjson::SizeType>(m_start.length()));
        writer.String("end");
        writer.String(m_end.c_str(), static_cast<rapidjson::SizeType>(m_end.length()));
        writer.String("process_title");
        writer.String(m_processTitle.c_str(), static_cast<rapidjson::SizeType>(m_processTitle.length()));
        writer.String("mem");
        writer.Int(m_mem);
    }

private:
    std::string m_hwId;
    std::string m_start;
    std::string m_end;
    std::string m_processTitle;
    int m_mem;
};

#endif