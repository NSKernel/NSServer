/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  HTCPCPLauer.cpp

  Compiled with RFC2324.
 ===================================================
  Author: NSKernel
==================================================*/

#include <string>

#include "HTCPCPLayer.h"
#include "HTTPLayer.h"

namespace HTCPCPLayer
{
	HTTPLayer::HTTPResponse TeapotCallback(HTTPLayer::HTTPRequest Request)
	{
		HTTPLayer::HTTPResponse ReturnResponse;
		ReturnResponse.HTTPStatus = 418;
		std::string ResponseBody = "HTCPCP 418\nYou are attempting to request \"" + Request.Path + "\" from a teapot.\nThe resulting entity body is short and stout.";
		ReturnResponse.HeaderKeys.push_back(HTTPLayer::HTTPKeyEntry{ "Content-Length", std::to_string(ResponseBody.size()) });
		ReturnResponse.HeaderKeys.push_back(HTTPLayer::HTTPKeyEntry{ "Content-Type", "text/plain" });
		return ReturnResponse;
	}
};