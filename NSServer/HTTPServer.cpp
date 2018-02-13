/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  HTTPServer.h

  HTTP server.
 ===================================================
  Author: NSKernel
==================================================*/

#include "Global.h"
#include "HTTPLayer.h"
#include "HTTPServer.h"
#include "HTCPCPLayer.h"

HTTPServer::HTTPServer(int Port) : TCPServer(Port)
{
	CallbackFunctions.empty();
}

HTTPLayer::HTTPResponse(*HTTPServer::FindCallback(std::string Path)) (HTTPLayer::HTTPRequest)
{
	if (CallbackFunctions.size() > 0)
	{
		for (unsigned int i = 1; i < CallbackFunctions.size(); i++)
		{
			if (!Path.compare(CallbackFunctions[i].Path))
				return CallbackFunctions[i].Function;
		}
		return CallbackFunctions[0].Function;
	}
	else
	{
		Logging::Log("ERROR", "No callback function registered.");
		return &HTCPCPLayer::TeapotCallback;
	}
}

void HTTPServer::RegisterCallbackFunction(std::string Path, HTTPLayer::HTTPResponse(*Function)(HTTPLayer::HTTPRequest))
{
	CallbackFunctions.push_back(Callback{ Path, Function });
}

void HTTPServer::Start() {
	SignalHandler::RegisterInterruptSignalHandler();
	while (!SignalHandler::ReadInterruptBit()) {
		TCPStream ClientStream = AcceptedClient();
		HTTPLayer::HTTPRequest Request = HTTPLayer::HTTPParseRequest(ClientStream);
		HTTPLayer::HTTPResponse (*CallbackFunction) (HTTPLayer::HTTPRequest) = FindCallback(Request.Path);
		HTTPLayer::HTTPResponse Response = (*CallbackFunction)(Request);
		ClientStream << HTTPLayer::HTTPMakeResponse(Response);
	}
	Logging::Log("STATUS", "HTTP server closed.");
}