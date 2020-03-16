#ifndef USER_H
#define USER_H

#include <string_view>
#include <string>

#include "rapidjson/prettywriter.h"

class User
{
public:
    User(std::string_view hwId, std::string_view userName)
        : m_hwId(hwId), m_userName(userName) {}

    std::string GetHWID() const { return m_hwId; }

    std::string GetUserName() const { return m_userName; }

    template <typename Writer>
    void Serialize(Writer& writer)
    {
        writer.String("hwId");
        writer.String(m_hwId.c_str(), static_cast<rapidjson::SizeType>(m_hwId.length()));
        writer.String("username");
        writer.String(m_userName.c_str(), static_cast<rapidjson::SizeType>(m_userName.length()));
    }

private:
    std::string m_hwId;
    std::string m_userName;
};

#endif
