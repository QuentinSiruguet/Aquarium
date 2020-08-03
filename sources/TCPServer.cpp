#include "TCPServer.h"

TCPServer::TCPServer(int Port)
{
	this->opt = 1;
	this->addrlen = sizeof(address);

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(Port);

	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->opt, sizeof(this->opt));
	bind(this->server_fd, (struct sockaddr*) & this->address, sizeof(this->address));
	listen(this->server_fd, 5);
}

void TCPServer::setLastIPName(std::string name)
{
	bool alreadyDefine = false;
	for (auto& a : this->addresses)
		if (a.first == name)
			alreadyDefine = true;
	if (this->addresses[this->addresses.size() - 1].first == "undefined" && !alreadyDefine)
		this->addresses[this->addresses.size() - 1].first = name;
}

void TCPServer::showIP()
{
	std::cout << std::endl << std::endl << "\t\t\t\t\tADDRESS NUMBER : " << this->addresses.size() << std::endl;
	for (auto& a : this->addresses)
		std::cout << "\t\t\t\t\t\tADDRESS KNOWN : " << a.first << std::endl;
	std::cout << std::endl << std::endl;
}

void TCPServer::returnString(std::string trame)
{
	if (send(this->new_socket, trame.c_str(), trame.size(), MSG_CONFIRM) == -1)
		std::cout << "ERROR::TCPSERVER::SENDING_TRAME:: " << trame << std::endl;
}

void TCPServer::sendStringByName(std::string trame, std::string name)
{
	close(new_socket);
	int place;
	for (int i = 0; i < this->addresses.size(); i++)
	{
		if (this->addresses[i].first == name)
		{
			this->address = this->addresses[i].second;
			this->addrlen = sizeof(address);
			this->new_socket = accept(this->server_fd, (struct sockaddr*) & this->address, (socklen_t*)&this->addrlen);
			if (send(this->new_socket, trame.c_str(), trame.size(), MSG_CONFIRM) == -1)
				std::cout << "ERROR::TCPSERVER::SENDING_TRAME:: " << trame << std::endl;
		}
		else
			std::cout << "ERROR::TCPSERVER::UNKNOWN_NAME:: " << name << std::endl;
	}
	if (this->addresses.size() == 0)
		std::cout << "ERROR::TCPSERVER::NOT_ANY_REGISTERED_NAME:: " << name << std::endl;
}

std::string TCPServer::recvString()
{
	char buffer[1024] = { 0 };
	close(new_socket);
	this->addrlen = sizeof(address);
	this->new_socket = accept(this->server_fd, (struct sockaddr*) & this->address, (socklen_t*)&this->addrlen);

	std::pair<std::string, sockaddr_in> addressStack;
	addressStack.first = "undefined";
	addressStack.second = this->address;
	bool alreadyExist = false;

	for (int i = 0; i < this->addresses.size(); i++)
		if (this->addresses[i].second.sin_addr.s_addr == addressStack.second.sin_addr.s_addr)
			alreadyExist = true;

	if (!alreadyExist)
		this->addresses.push_back(addressStack);


	this->valread = recv(this->new_socket, buffer, 1024, MSG_PEEK);
	return buffer;
}
