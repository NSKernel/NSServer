/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  main.cpp

  Entry point of the server.
 ===================================================
  Author: NSKernel
==================================================*/

#include <iostream>
#include <string>
#include <sstream>

#include "FileSystem.h"
#include "HTTPLayer.h"
#include "HTTPServer.h"
#include "Global.h"
//#include <boost/python.hpp>

std::string BaseDirectory;
//using namespace boost::python;

HTTPLayer::HTTPResponse Generate404Response(HTTPLayer::HTTPRequest Request)
{
	HTTPLayer::HTTPResponse ReturnResponse;
	ReturnResponse.HTTPStatus = 404;
	std::string ResponseBody = "HTTP 404\nRequested file \"" + Request.Path + "\" not found on the server.";
	ReturnResponse.HeaderKeys.push_back(HTTPLayer::HTTPKeyEntry{ "Content-Length", std::to_string(ResponseBody.size()) });
	ReturnResponse.HeaderKeys.push_back(HTTPLayer::HTTPKeyEntry{ "Content-Type", "text/plain" });
	return ReturnResponse;
}

HTTPLayer::HTTPResponse FileCallback(HTTPLayer::HTTPRequest Request)
{
	HTTPLayer::HTTPResponse ReturnResponse;
	
	std::string RealPath = BaseDirectory + Request.Path;
	File RequestedFile = File(RealPath);
	if (!RequestedFile.IsExists())
		return Generate404Response(Request);
	if (RequestedFile.IsDirectory())
	{
		RealPath += (RealPath[RealPath.size() - 1] == '/' ? "" : "/");
		RealPath += "index.html";
		RequestedFile = File(RealPath);
		if (!RequestedFile.IsExists())
			return Generate404Response(Request);
	}

	ReturnResponse.HTTPStatus = 200;
	ReturnResponse.Data = RequestedFile.ReadAll();
	
	// Build header
	std::string FileExtensionName = RequestedFile.FileExtensionName();
	auto Iterator = HTTPLayer::HTTPFileType.find(FileExtensionName);
	if (Iterator != HTTPLayer::HTTPFileType.end())
		ReturnResponse.HeaderKeys.push_back(HTTPLayer::HTTPKeyEntry{ "Content-Type", Iterator->second });
	else
		ReturnResponse.HeaderKeys.push_back(HTTPLayer::HTTPKeyEntry{ "Content-Type", "application/octet-stream" });
	
//	python::Py_Initialize();
//	PyRun_SimpleString("print 'hello'");
//	Py_Finalize();

	ReturnResponse.HeaderKeys.push_back(HTTPLayer::HTTPKeyEntry{ "Content-Length", std::to_string(ReturnResponse.Data.size()) });


	return ReturnResponse;
}

HTTPLayer::HTTPResponse AddCallback(HTTPLayer::HTTPRequest Request)
{
	HTTPLayer::HTTPResponse ReturnResponse;

	ReturnResponse.HTTPStatus = 200;
	ReturnResponse.HeaderKeys.push_back(HTTPLayer::HTTPKeyEntry{ "Content-Type", "text/plain" });
	
	std::string ResponseString;
	if (Request.GetData.size() != 2)
	{
		ResponseString = "Invalid amount of parameters. Usage: /add?num1=xxxxxx&num2=xxxxxx";
	}
	else
	{
		ResponseString = std::to_string(atoi(Request.GetData[0].KeyData.c_str())) 
			+ " + " + std::to_string(atoi(Request.GetData[1].KeyData.c_str())) 
			+ " = " + std::to_string(atoi(Request.GetData[0].KeyData.c_str()) 
				+ atoi(Request.GetData[1].KeyData.c_str()));
	}

	ReturnResponse.HeaderKeys.push_back(HTTPLayer::HTTPKeyEntry{ "Content-Length", std::to_string(ResponseString.size()) });
	ReturnResponse.Data = ResponseString;

	return ReturnResponse;
}

int main()
{
	std::cout << "NSServer [Version 0.1]\nCopyright (C) 2018 NSKernel. All rights reserved.\n\nThis project is licensed under GPL v3.\n\nSETUP: What is the base directory of the server?\nBase directory> ";
//	std::cin >> BaseDirectory;
	BaseDirectory = "/home/broccoli/assiduous/";
	Logging::Log("STATUS", "Base directory is set to " + BaseDirectory);
	HTTPServer Server(8080);
	Server.RegisterCallbackFunction("Default", FileCallback);
	Server.RegisterCallbackFunction("add", AddCallback);
	Server.Start();
	return 0;
}