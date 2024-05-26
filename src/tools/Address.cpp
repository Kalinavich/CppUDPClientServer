#include "../../include/Address.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <charconv>

namespace Tools
{
    Address::Address(uint32_t ip, uint16_t port)
    {
        this->ip = ip;
        this->port = port;
    }

    uint16_t Address::GetPort() 
    {
        return port;
    }

    uint32_t Address::GetIp() 
    {
        return ip;
    }

    std::optional<uint32_t> Address::parseIPString(const std::string& str)
    {
        in_addr addr;
        const auto res = inet_pton(AF_INET, str.c_str(), &addr);
        if (res == 0) {
            return std::nullopt;
        }
        
        return addr.s_addr;
    }

    std::optional<Address> Address::getAddressFromString(const std::string& str)
    {
        const auto portDelim = str.find(':');
        if (portDelim == std::string::npos)
            return std::nullopt;
        
        const auto ipStr = str.substr(0, portDelim);
        const auto ip = parseIPString(ipStr);
        if (!ip) 
            return std::nullopt;
        
        uint16_t port = 0;
        const auto portSv = std::string_view { str }.substr(portDelim + 1);
        const auto [ptr, ec] = std::from_chars(&portSv.front(), &portSv.back() + 1, port);
        if (ec != std::errc() || ptr != &portSv.back() + 1)
            return std::nullopt;
        
        return Address(*ip, port);
    }
}