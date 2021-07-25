// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include<iostream>
#include<stdint.h>
#include "logger.h"
#define PORT 8080
#define LOGTYPE "client"
#define BLOCK_SIZE 1024
using namespace std;

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		logger("socket creation failed", LOGTYPE);
		return -1;
	}
	logger("Socket creation successful", LOGTYPE);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		logger("Invalid address/ Address not supported", LOGTYPE);
		return -1;
	}
	logger("Valid server address", LOGTYPE);
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		logger("Failed to connect to server", LOGTYPE);
		return -1;
	}
	logger("Successfully connected to server", LOGTYPE);

    int32_t filesize = 0;
	char filename[100];
	scanf("%s",filename);

	//send filename to server
    if(send(sock, filename, 100, 0) <= 0){
		logger("Failed to send filename", LOGTYPE);
		exit(EXIT_FAILURE);
	}
	logger("sent filename to server", LOGTYPE);
	int retval = recv(sock, &filesize, sizeof(int32_t),0);
	if(retval <= 0){
		cout<<"Read failure"<<"\n";
		logger("Failed to read data from server", LOGTYPE);
	}
	else{
		string msg = "Receiving file of size: " + to_string(filesize) + ", preparing buffer";
		logger(msg, LOGTYPE);
		cout<<"Receiving File of Size: "<<filesize<<" Bytes"<<endl;
	}

	//read the received file contents
	char buffer[filesize+1];
	if(read(sock, buffer, filesize) < 0){
		logger("Failed to read filedata", LOGTYPE);
		exit(EXIT_FAILURE);
	}
    buffer[filesize] = '\0';
	logger("Read successful", LOGTYPE);

	//display the file contentss
	cout<<buffer<<"\n";
	return 0;
}
