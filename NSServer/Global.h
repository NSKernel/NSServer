/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  Global.h

  Global definitions.
 ===================================================
  Author: NSKernel
==================================================*/

#include <iostream>

#ifndef GLOBAL_H
#define GLOBAL_H

namespace Logging
{
	void Log(std::string Type, std::string Content);
}

class SignalHandler {
	static bool InterruptBit;
public:
	static bool ReadInterruptBit();
	static void InterruptSignalHandler(int SignalNumber);
	static void RegisterInterruptSignalHandler();
};

bool PeekUntil(
	std::istream &InStream,
	std::string &SplitedString,
	std::string Chars
);

#endif