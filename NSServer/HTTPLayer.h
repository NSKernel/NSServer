/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  HTTPLayer.h

  Process HTTP responses and requests.
 ===================================================
  Author: NSKernel
==================================================*/

#include <string>
#include <vector>
#include <map>


#ifndef HTTPLAYER_H
#define HTTPLAYER_H

namespace HTTPLayer
{
	enum HTTPMethod
	{
		GET,
		POST,
		PUT,
		PATCH,
		DELETE,
		OPTIONS,
		HEAD,
		TRACE,
		CONNECT
	};

	const std::map<int, std::string> HTTPResponseCode = {
		{ 200, "OK" },
		{ 304, "Not Modified"},
		{ 400, "Bad Request" },
		{ 403, "Forbidden" },
		{ 404, "Not Found" },
	    { 418, "I'm a teapot" }
	};

	const std::map<std::string, std::string> HTTPFileType = {
		{ "html","text/html" },
		{ "css" ,"text/css" },
		{ "js"  ,"application/js" },
		{ "json","application/json" },
		{ "jpg" ,"image/jpg" },
		{ "jpeg","image/jpeg" },
		{ "png" ,"image/png" },
		{ "ico" ,"image/ico" },
		{ "gif" ,"image/gif" },
		{ "txt", "text/plain" },
		{ "mp3", "audio/mp3" },
		{ "ttf", "font/ttf" }
	};

	struct HTTPKeyEntry
	{
		std::string KeyName;
		std::string KeyData;
	};

	struct HTTPRequest
	{
		bool IsBadRequest;
		HTTPMethod Method;
		std::string Path;
		std::vector<HTTPKeyEntry> GetData;
		std::vector<HTTPKeyEntry> HeaderKeys;

		std::string PostData; // Optional
	};

	struct HTTPResponse
	{
		int HTTPStatus;
		std::vector<HTTPKeyEntry> HeaderKeys;

		std::string Data;
	};

	std::string HTTPMakeResponse(HTTPResponse Response);
	HTTPRequest HTTPParseRequest(std::istream &RequestStream);

};

#endif
