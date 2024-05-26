
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
#include <thread>
#include <pthread.h>

int main(int argc, char *argv[])
{
    std::unordered_map<std::string, std::string> resources;

    std::string line;
 
    std::ifstream in("resources.txt");

    if (!in.is_open())
        return 1;

    std::cout << "Resources:\n";

    while (std::getline(in, line))
    {
        auto npos = line.find(' ');
        if (npos == std::string::npos)
            continue;

        std::cout << "\t" << line << std::endl;
        auto key = line.substr(0, npos);
        auto value = line.substr(npos, line.size()-1);
        resources[key] = value;         
    }  
    in.close();

    if (!!!resources.size()) 
        return 1; 

   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[64];

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) fprintf(stderr, "ERROR create socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock, reinterpret_cast<sockaddr *>(&server),length) < 0) 
       fprintf(stderr, "ERROR Bind socket error");
   fromlen = sizeof(sockaddr_in);

   for (;;) {
       n = recvfrom(sock,buf,64,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) fprintf(stderr, "ERROR recvfrom");

       std::string b (buf, n); 

       const auto val = resources.find(b);

       std::cout << b << std::endl;

       if (val == resources.end())
       {
            n = sendto(sock,"-ERROR-\nResource is not found\n-END-",35,
                0,(struct sockaddr *)&from,fromlen);
            if (n  < 0) fprintf(stderr, "ERROR sendToError");
            continue;
       }

       auto response = "-BEGIN-\n" + val->second + "\n-END-";       

       n = sendto(sock,response.c_str(),response.size(),
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) fprintf(stderr, "ERROR sendto");

   }
   return 0;
 }
