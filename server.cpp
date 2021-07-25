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
#define PORT 8080
#define BLOCK_SIZE 1024
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
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

    recv(new_socket, filename, 100, 0);
    cout<<"Received filename: "<<filename<<"\n";
        
    //filename is present in filename, server will search for the file in current directory
    string path = "/home/laxus/c++/sockets";
    string no_found = "Sorry, file not found";
    string yes_found = "File found";
    DIR *dir;
    struct dirent *diread;
    bool found = false;
    
    if( (dir = opendir("/home/laxus/c++/sockets")) != nullptr){
        cout<<"Scanning directory for file "<<filename<<"\n";
        while((diread = readdir(dir)) != nullptr){
            //files.push_back(diread->d_name);
            if(strcmp(filename, diread->d_name) == 0){
                cout<<yes_found<<endl;
                found = true;
            }
        }
        closedir(dir);
        if(!found){
            cout<<no_found<<endl;
        }     
    }
    else {
        perror ("opendir");
        return EXIT_FAILURE;
    }

    if(true){ //yes_found
        //start reading the file
        ifstream fr;
        string filedata = "";
        char current_char = 0;
        int idx = 0;

        fr.open(filename);
        while(fr.get(current_char)){
            filedata.push_back(current_char);
        }
        fr.close();
        
        int32_t l = filedata.length();
        //send file size
        send(new_socket, &l, sizeof(int32_t), 0);

        //send file data
        send(new_socket, filedata.c_str(), filedata.length(), 0);
    }
	return 0;
}
