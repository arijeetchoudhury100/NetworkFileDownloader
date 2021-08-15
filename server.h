#include<string>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SERVER_H
#define SERVER_H

class SocketServer
{
    private:
        std::string hostname;
        int port;
        int server_fd;
        int new_socket;
        int addrlen;
        int opt;
        bool isChildPresent;
        struct sockaddr_in address;
        
    public:
        SocketServer(std::string hostname, int port);
        int init();
        int set_socket_options();
        int socket_bind();
        int socket_listen_and_accept();
        int socket_accept();
        int find_child(std::string parent, std::string child);
        int read_file(std::string parent, std::string child, std::string& buf);
        int send_file(std::string parent, std::string child);
        int send_metadata(int32_t len);
        int send_data(std::string buf);
};
#endif