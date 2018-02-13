/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  StringHelper.cpp

  Helpers to process string stream.
 ===================================================
  Author: NSKernel
==================================================*/

#include <array>
#include <istream>
#include <type_traits>
#include <cctype>
#include <utility>

bool Contains(const std::string OriginalString, const char val) {
	for (unsigned int i = 0; i < OriginalString.size(); i++)
		if (OriginalString[i] == val)
			return true;
	return false;
}

bool PeekUntil(
	std::istream &InStream,
	std::string &SplitedString,
	std::string Chars
)
{
	SplitedString = "";
	while (InStream.good()) {
		if (Contains(Chars, (char)InStream.peek()))
		{
			// Kill the flag char
			InStream.get();
			return true;
		}
		SplitedString.push_back((char)InStream.get());
	}
	return false;
}