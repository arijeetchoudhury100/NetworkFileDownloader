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
#define PORT 8080
#define BLOCK_SIZE 1024
#define LOGTYPE "server"
using namespace std;

int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char filename[100];
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
        logger("socket creation failed", LOGTYPE);
		exit(EXIT_FAILURE);
	}
	else{
        logger("bsocket creation successful", LOGTYPE);
    }
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
        logger("failed to attach socket to port", LOGTYPE);
		exit(EXIT_FAILURE);
	}
    else{
        logger("successfully attached socket to port", LOGTYPE);
    }
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
        logger("failed to bind socket", LOGTYPE);
		exit(EXIT_FAILURE);
	}
    else{
        logger("bind successful for socket", LOGTYPE);
    }

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
        logger("failed to listen to port", LOGTYPE);
		exit(EXIT_FAILURE);
	}
    else{
        logger("Listening for incoming connection...", LOGTYPE);
    }
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
        logger("failed to accept connection from client", LOGTYPE);
		exit(EXIT_FAILURE);
	}
    else{
        logger("Client connected successfully", LOGTYPE);
    }
    recv(new_socket, filename, 100, 0);
    logger("Received filename from client: ", LOGTYPE);
    cout<<"Received filename: "<<filename<<"\n";
        
    //filename is present in filename, server will search for the file in current directory
    string path = "/home/laxus/c++/sockets";
    string no_found = "Sorry, file not found";
    string yes_found = "File found";
    DIR *dir;
    struct dirent *diread;
    bool found = false;
    
    if( (dir = opendir("/home/laxus/c++/sockets")) != nullptr){
        logger("scanning pwd for file", LOGTYPE);
        cout<<"Scanning directory for file "<<filename<<"\n";
        while((diread = readdir(dir)) != nullptr){
            //files.push_back(diread->d_name);
            if(strcmp(filename, diread->d_name) == 0){
                cout<<yes_found<<endl;
                found = true;
                logger("File found, proceeding to transfer", LOGTYPE);
            }
        }
        closedir(dir);
        if(!found){
            cout<<no_found<<endl;
            logger("File not found", LOGTYPE);
        }     
    }
    else {
        perror ("opendir");
        logger("Failed to open directory", LOGTYPE);
        return EXIT_FAILURE;
    }

    if(found){ //yes_found
        //start reading the file
        logger("Started file read", LOGTYPE);
        ifstream fr;
        string filedata = "";
        char current_char = 0;
        int idx = 0;

        fr.open(filename);
        while(fr.get(current_char)){
            filedata.push_back(current_char);
        }
        logger("File read successful", LOGTYPE);
        fr.close();
        
        int32_t l = filedata.length();
        //send file size
        string msg = "Sending file size to client, size of file is" + to_string(l);
        logger(msg,LOGTYPE);

        if(send(new_socket, &l, sizeof(int32_t), 0) <= 0){
            logger("Failed to send filesize", LOGTYPE);
            exit(EXIT_FAILURE);
        }
        logger("Filesize sent to client", LOGTYPE);
        //send file data
        logger("Sending file data to client", LOGTYPE);
        if(send(new_socket, filedata.c_str(), filedata.length(), 0) <= 0){
            logger("Failed to send filedata", LOGTYPE);
            exit(EXIT_FAILURE);
        }
        logger("File data sent to client!", LOGTYPE);
    }
	return 0;
}
