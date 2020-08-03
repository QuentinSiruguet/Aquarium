#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>

#include <unistd.h> 
//#include <stdio.h> 
//#include <stdlib.h> 
//#include <string.h> 


class TCPServer
{
private:
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt;
	int addrlen;
	std::vector <std::pair <std::string, sockaddr_in>> addresses;

public:
	TCPServer(int Port);

	void setLastIPName(std::string name);
	void showIP();

	void returnString(std::string trame);
	void sendStringByName(std::string trame, std::string name);
	std::string recvString();
};

