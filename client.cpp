// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include<iostream>
#include<stdint.h>
#define PORT 8080
#define BLOCK_SIZE 1024
using namespace std;

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

    int32_t filesize = 0;
	char filename[100];
	scanf("%s",filename);

	//send filename to server
    send(sock, filename, 100, 0);
	int retval = recv(sock, &filesize, sizeof(int32_t),0);
	if(retval <= 0)
		cout<<"Read failure"<<"\n";
	else
		cout<<"Receiving File of Size: "<<filesize<<" Bytes"<<endl;

	//read the received file contents
	char buffer[filesize+1];
	read(sock, buffer, filesize);
    buffer[filesize] = '\0';

	//display the file contentss
	cout<<buffer<<"\n";
	return 0;
}
