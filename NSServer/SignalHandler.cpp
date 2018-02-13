/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  SignalHandler.cpp

  Signal handler.
 ===================================================
  Author: NSKernel
==================================================*/

#include <signal.h>

#include "Global.h"

bool SignalHandler::InterruptBit = false;

bool SignalHandler::ReadInterruptBit() {
	return InterruptBit;
}

void SignalHandler::InterruptSignalHandler(int SignalNumber) {
	Logging::Log("SIGNAL", "Keyboard signal received.");
	InterruptBit = true;
}

void SignalHandler::RegisterInterruptSignalHandler() {
	signal(SIGINT, InterruptSignalHandler);
}