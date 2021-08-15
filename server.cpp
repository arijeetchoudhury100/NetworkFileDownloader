// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <dirent.h> 
#include <iostream>
#include <vector>
#include <stdint.h>
#include <fstream>
#include "logger.h"
#include "server.h"

#define PORT 8080
#define BLOCK_SIZE 1024
#define LOGTYPE "server"
using namespace std;

SocketServer::SocketServer(string hostname, int port)
{
    this->port = port;
    this->hostname = hostname;
    opt = 1;
    isChildPresent = false;
}

int SocketServer::init()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("socket creation failed");
        logger("Failed to create server socket", LOGTYPE);
        return -1;
    }
    address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
    addrlen = sizeof(address);
    logger("server socket init success", LOGTYPE);
    return 0;
}

int SocketServer::socket_bind()
{
    if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
        logger("failed to bind socket", LOGTYPE);
		return -1;
	}
    logger("successfully bound to port",LOGTYPE);
    return 0;
}

int SocketServer::socket_listen_and_accept()
{
    if (listen(server_fd, 3) < 0)
	{
		perror("listen");
        logger("Failed to listen to port", LOGTYPE);
		return -1;
	}
    logger("Listening for incoming connection...", LOGTYPE);
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
        logger("Failed to accept connection from client", LOGTYPE);
        return -1;
	}
    logger("Client connected successfully", LOGTYPE);
    return 0;
}

int SocketServer::set_socket_options()
{
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
        logger("failed to set socket options", LOGTYPE);
        return -1;
	}
    
    logger("successfully set socket options", LOGTYPE);
    
}

int SocketServer::find_child(string parent, string child)
{
    DIR *dir;
    struct dirent *diread;
    bool found = false;
    
    if( (dir = opendir(parent.c_str())) != nullptr)
    {
        logger("scanning parent for file", LOGTYPE);
        while((diread = readdir(dir)) != nullptr)
        {
            if(strcmp(child.c_str(), diread->d_name) == 0)
            {
                isChildPresent = true;
                logger("File found, proceeding to transfer", LOGTYPE);
            }
        }
        closedir(dir);
        if(!isChildPresent)
        {
            logger("File not found", LOGTYPE);
            return 0;
        }
        return 1;     
    }
    else 
    {
        perror ("opendir");
        logger("Failed to open directory", LOGTYPE);
        return 0;
    }
}

int SocketServer::read_file(string parent, string child, string& buf)
{
    if (!find_child(parent, child))
    {
        logger("Failed to read file, file not present", LOGTYPE);
        return 0;
    }
    
    logger("Started file read", LOGTYPE);
    
    ifstream fr;
    char current_char = 0;
    fr.open(child);
    while(fr.get(current_char)){
        buf.push_back(current_char);
    }
    
    logger("File read successful", LOGTYPE);
    fr.close();
    return 1;
}

int SocketServer::send_metadata(int32_t len)
{
    string msg = "Sending file size to client, size of file is" + to_string(len);
    logger(msg,LOGTYPE);
    if (send(new_socket, &len, sizeof(int32_t), 0) <= 0){
        logger("Failed to send filesize", LOGTYPE);
        return 0;
    }
    logger("Filesize sent to client", LOGTYPE);
    return 1;
}

int SocketServer::send_data(string buf)
{
    string msg = "Sending file data to client, size of file is" + to_string(buf.length());
    logger(msg,LOGTYPE);
    if( send(new_socket, buf.c_str(), sizeof(int32_t), 0) <= 0){
        logger("Failed to send file data", LOGTYPE);
        return 0;
    }
    logger("File data sent to client", LOGTYPE);
    return 1;
}

int SocketServer::send_file(string parent, string child)
{
    string buf = "";
    if (!read_file(parent,child,buf))
    {
        logger("Failed to send file to client, can't read file", LOGTYPE);
        return 0;
    }

    int32_t len = buf.length();
    //send file metadata ( length of file )
    if (!send_metadata(len))
    {
        logger("Failed to send metadata", LOGTYPE);
        return 0;
    }

    if (!send_data(buf))
    {
        logger("Failed to send file data", LOGTYPE);
        return 0;
    }

    //file send successfully
    return len;

}
int main(int argc, char const *argv[])
{
        SocketServer server("127.0.0.1", PORT);
        server.init();
        server.set_socket_options();
        server.socket_bind();
        server.socket_listen_and_accept();
		return 0;
}
