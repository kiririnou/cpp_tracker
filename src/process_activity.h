#ifndef RESOURCE_USAGE_H
#define RESOURCE_USAGE_H

#include "rapidjson/prettywriter.h"
#include <string>

class ProcessActivity
{
public:
    ProcessActivity() {}

    // ProcessActivity(const std::string& hwId, const std::string& start, const std::string& end, const std::string& processTitle, int mem)
    //     : m_hwId(hwId), m_start(start), m_end(end), m_processTitle(processTitle), m_mem(mem) {} 
    
    ProcessActivity(const std::string& hwId, const std::string& start, const std::string& processTitle, int mem)
        : m_hwId(hwId), m_start(start), m_processTitle(processTitle), m_mem(mem) {}
        
    ~ProcessActivity() {}

    void SetHWID(const std::string& hwId)
    {
        m_hwId = hwId;
    }

    void SetStartTime(const std::string& start)
    {
        m_start = start;
    }

    void SetEndTime(const std::string& end)
    {
        m_end = end;
    }

    void SetProcessTitle(const std::string& processTitle)
    {
        m_processTitle = processTitle;
    }

    void SetMemory(int mem)
    {
        m_mem = mem;
    }

    std::string GetHWID() const
    {
        return m_hwId;
    }

    std::string GetStartTime() const
    {
        return m_start;
    }

    std::string GetEndTime() const
    {
        return m_end;
    }

    std::string GetProcessTitle() const
    {
        return m_processTitle;
    }

    int GetMemory() const
    {
        return m_mem;
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