#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <arpa/inet.h>
#include <optional>
#include <charconv>

#include "../../include/Socket.h"

class Address {
public:
    Address() = delete;
    Address(uint32_t ip, uint16_t port)
    {
        this->ip = ip;
        this->port = port;
    }

    uint16_t GetPort() 
    {
        return port;
    }

    uint32_t GetIp() 
    {
        return ip;
    }

private:

    uint32_t ip;
    uint16_t port;
};

std::optional<uint32_t> parseIPString(const std::string& str)
{
    in_addr addr;
    const auto res = inet_pton(AF_INET, str.c_str(), &addr);
    if (res == 0) {
        return std::nullopt;
    }
    
    return addr.s_addr;
}

std::optional<Address> getAddressFromString(const std::string& str)
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

void Error(const char* message, int sock)
{
    std::cerr << message << std::endl;
    if (sock > 0)
        close(sock);
    exit(0);
}

void Error(const char* message)
{
    Error(message, -1);
}

int main(int argc, char *argv[])
{

   int length, n;
   socklen_t fromlen;
   struct sockaddr_in server;

   if (argc != 3)
      Error("Error: Please enter the arguments: host name, port number and request name resource in the form HOST:PORT RESOURCE");
 
   if (strlen(argv[2]) > 64)
      Error("Error: Please enter RESOURCE no more than 64 byte");
   
   auto address = getAddressFromString(argv[1]);

   if (!address)
      Error("Error: Incorrectable address");

   Server::Socket sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) 
      Error("Error: create socket", sock);

   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr = address->GetIp();
   server.sin_port=htons(address->GetPort());
  
   n = sendto(sock, argv[2] ,strlen(argv[2]),
                0,(struct sockaddr *)&server, length);

   if (n < 0) 
      Error("Error: sendto", sock);
   
   int sizeBuf = 1024;
   char buf[sizeBuf];
   fromlen = sizeof(sockaddr_in);

    timeval timeout;      
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    
    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0)
        Error("Error: setsockopt", sock);

   n = recvfrom(sock,buf,sizeBuf,0,reinterpret_cast<sockaddr *>(&server), &fromlen);
   if (n < 0) Error("Error: recvfrom. Timeout", sock);            

   std::string b (buf, n); 

   if (b.find("-ERROR-") != std::string::npos)
      Error(b.c_str(), sock);
   
   std::cout << b << std::endl;

   close(sock);

   return 0;
 }
