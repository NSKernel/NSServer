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

#include <string>
#include <vector>
#include <map>

#include "HTTPLayer.h"
#include "TCPLayer.h"

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

struct Callback 
{
	std::string Path;
	HTTPLayer::HTTPResponse (*Function)(HTTPLayer::HTTPRequest);
};

class HTTPServer : private TCPServer
{
private:
	std::vector<Callback> CallbackFunctions;

public:
	HTTPServer(int Port = 80);
	void RegisterCallbackFunction(std::string Path, HTTPLayer::HTTPResponse(*Function)(HTTPLayer::HTTPRequest));
	HTTPLayer::HTTPResponse (*FindCallback(std::string Path)) (HTTPLayer::HTTPRequest);
	void Start();
};

#endif
