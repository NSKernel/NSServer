/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  TCPServer.cpp

  Provide TCP server.
 ===================================================
  Author: NSKernel
==================================================*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sstream>

#include <map>
#include <cctype>
#include <algorithm>
#include <memory>

#include "TCPLayer.h"
#include "Global.h"

TCPServer::TCPServer(int Port)
{
	this->Port = Port;
	this->ServerFileDescriptor = -1;
	InitializeServerFileDescriptor();
}


TCPServer::~TCPServer() 
{
	if (ServerFileDescriptor > 0) 
	{
		std::stringstream LogContent;
		LogContent << "Server closed with file descriptor " << ServerFileDescriptor << ".";
		Logging::Log("STATUS", LogContent.str());
		close(ServerFileDescriptor);
	}
}

void TCPServer::InitializeServerFileDescriptor()
{
	ServerFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerFileDescriptor < 0) 
	{
		Logging::Log("ERROR", "Failed to create socket.");
	}

	// create sockaddr
	struct sockaddr_in ServerAddress;
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons((uint16_t)Port);
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ServerFileDescriptor, (struct sockaddr *)&ServerAddress, sizeof(ServerAddress)) == -1) 
	{
		std::stringstream LogContent;
		LogContent << "Can not bind to port " << Port << ".";
		Logging::Log("ERROR", LogContent.str());
	}

	// FIXME:
	if (listen(ServerFileDescriptor, 50) == -1) 
	{
		Logging::Log("ERROR", "Failed to listen on socket.");
	}
}


void TCPServer::InitializeServerFileDescriptor(int Port) 
{
	// re-init
	this->~TCPServer();
	this->Port = Port;
	InitializeServerFileDescriptor();
}


TCPStream TCPServer::AcceptedClient()
{
	struct sockaddr_in ClientAddress;
	socklen_t Length = sizeof(ClientAddress);
	int Connection = accept(ServerFileDescriptor, (struct sockaddr*)&ClientAddress, &Length);
	std::stringstream LogContent;
	LogContent << "Client Address " << inet_ntoa(ClientAddress.sin_addr) << ":" << ClientAddress.sin_port << ", Connection:" << Connection << ".";
	Logging::Log("STATUS", LogContent.str());
	if (Connection < 0)
	{
		Logging::Log("ERROR", "Failed to accept client.");
	}

	return TCPStream(Connection);
}