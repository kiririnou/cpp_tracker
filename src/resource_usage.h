#ifndef RESOURCE_ACTIVITY_H
#define RESOURCE_ACTIVITY_H

#include <string>
#include <string_view>

#include "rapidjson/prettywriter.h"

class ResourceUsage
{
public:
    ResourceUsage(std::string_view time, int cpu, int mem)
        : m_time(time), m_cpu(cpu), m_mem(mem) {}

	std::string GetTime() const { return m_time; }

	int GetCPU() const { return m_cpu; }

	int GetMemory() const { return m_mem; }

    template <typename Writer>
    void Serialize(Writer& writer)
    {
        writer.String("time");
        writer.String(m_time.c_str(), static_cast<rapidjson::SizeType>(m_time.length()));
        writer.String("cpu");
        writer.Int(m_cpu);
        writer.String("mem");
        writer.Int(m_mem);
    }

private:
    std::string m_time;
    int m_cpu;
    int m_mem;
};

#endif
