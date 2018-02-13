/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  Logging.cpp

  Log status.
 ===================================================
  Author: NSKernel
==================================================*/

#include <iostream>
#include <string>
#include <ctime>

#include "Global.h"

#define _CRT_SECURE_NO_WARNINGS 

namespace Logging
{

	void Log(std::string Type, std::string Content)
	{
		time_t timep;
		time(&timep);
		char Time[100];
		strftime(Time, 100, "%c", gmtime(&timep));
		std::cout << Type << " : " << Time << " : " << Content << std::endl;
	}
}