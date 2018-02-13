/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  HTTPLayer.cpp

  Process HTTP responses and requests.
 ===================================================
  Author: NSKernel
==================================================*/

#include <sys/types.h>
#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <cstring>
#include <cctype>

#include "Global.h"
#include "HTTPLayer.h"

namespace HTTPLayer
{
	std::string HTTPMakeResponse(HTTPResponse Response)
	{
		std::stringstream RStream;
		std::stringstream LogStream;
		std::string StatusCode;
		StatusCode = HTTPResponseCode.find(Response.HTTPStatus)->second;
		LogStream << "Response sent with " << Response.HTTPStatus << " " << StatusCode << ".";
		RStream << "HTTP/1.1 " << Response.HTTPStatus << " " << StatusCode << "\r\n";
		for (unsigned int i = 0; i < Response.HeaderKeys.size(); i++)
		{
			RStream << Response.HeaderKeys[i].KeyName << ": " << Response.HeaderKeys[i].KeyData << "\r\n";
		}
		RStream << "\r\n" << Response.Data;
		Logging::Log("STATUS", LogStream.str());
		return RStream.str();
	}

	HTTPRequest HTTPParseRequest(std::istream &RequestStream)
	{
		HTTPRequest ParsedRequest;
		std::string Token;
		std::string Dummy;

		ParsedRequest.IsBadRequest = false;

		// METHOD
		RequestStream >> Token;
		if (!Token.compare("GET"))
			ParsedRequest.Method = GET;
		else if (!Token.compare("POST"))
			ParsedRequest.Method = POST;
		else
		{
			ParsedRequest.IsBadRequest = true;
			Logging::Log("ERROR", "Bad request detected. Unhandled request method " + Token + ".");
			return ParsedRequest;
		}


		// PATH?ARGUMENTS
		RequestStream >> Token;
		std::istringstream PathStream(Token);
		std::getline(PathStream, ParsedRequest.Path, '?');
		if (ParsedRequest.Path[0] == '/')
			ParsedRequest.Path.erase(0, 1);
		std::string ParameterList;
		std::getline(PathStream, ParameterList, ' ');
		std::istringstream ArgumentStream(ParameterList);
		std::string Argument;
		do
		{
			if (!std::getline(ArgumentStream, Argument, '&'))
				break;
			if (Argument.compare(""))
			{
				HTTPKeyEntry KeyEntry;
				std::istringstream ArgumentEntryStream(Argument);
				std::getline(ArgumentEntryStream, KeyEntry.KeyName, '=');
				std::getline(ArgumentEntryStream, KeyEntry.KeyData, ' ');
				ParsedRequest.GetData.push_back(KeyEntry);
			}
		} while (true);
		RequestStream.get(); // Jump ' '

		// HTTP Version -- assert 1.1 or 2
		if (!PeekUntil(RequestStream, Token, std::string("\r")) || (Token.compare("HTTP/1.1") && Token.compare("HTTP/2")) || !PeekUntil(RequestStream, Dummy, std::string("\n")) || Dummy.compare("")) // Must have a \n
		{
			ParsedRequest.IsBadRequest = true;
			Logging::Log("ERROR", "Bad request detected. Unhandled HTTP Version " + Dummy + ".");
			return ParsedRequest;
		}

		// Header Keys
		std::string HeaderKey;
		do
		{
			if (!PeekUntil(RequestStream, HeaderKey, std::string("\r")) || !HeaderKey.compare(""))
				break;
			if (HeaderKey.compare(""))
			{
				HTTPKeyEntry KeyEntry;
				std::istringstream HeaderKeyEntryStream(HeaderKey);
				std::getline(HeaderKeyEntryStream, KeyEntry.KeyName, ':');
				std::getline(HeaderKeyEntryStream, Dummy, ' '); // ": "
				std::getline(HeaderKeyEntryStream, KeyEntry.KeyData, '\0');
				ParsedRequest.HeaderKeys.push_back(KeyEntry);
			}
			PeekUntil(RequestStream, Dummy, std::string("\n"));
		} while (true);
		PeekUntil(RequestStream, Dummy, std::string("\n"));

		// Data
		std::string LengthString = "0";
		for (unsigned int i = 0; i < ParsedRequest.HeaderKeys.size(); i++)
		{
			if (!ParsedRequest.HeaderKeys[i].KeyName.compare("Content-Length"))
			{
				LengthString = ParsedRequest.HeaderKeys[i].KeyData;
				break;
			}
		}
		ssize_t Length = atoi(LengthString.c_str());
		while (Length && RequestStream.good()) 
		{
			ParsedRequest.PostData.push_back((char)RequestStream.get());
			Length--;
		}
		if (Length != 0)
		{
			ParsedRequest.IsBadRequest = true;
			Logging::Log("ERROR", "Bad request detected. Data length is wrong.");
			return ParsedRequest;
		}
		return ParsedRequest;
	}
};