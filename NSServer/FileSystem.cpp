/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  FileSystem.cpp

  Provide file system support.
 ===================================================
  Author: NSKernel
==================================================*/

#include <cstdlib>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <fstream>

#include "FileSystem.h"

File::File() :
	FileName(),
	FileStatus()
{
}

File::File(const std::string &FileName) : FileStatus(new struct stat)
{
	this->FileName = FileName;
	// this->FileStatus = *new std::shared_ptr<stat>(new struct stat);
	if (lstat(FileName.c_str(), FileStatus.get()) < 0)
		FileStatus.reset();
}

std::string File::FileExtensionName() 
{
	auto pos = FileName.find_last_of("/.");
	if (pos == FileName.npos || FileName[pos] == '/')
		return "";
	return FileName.substr(pos + 1);
}

std::string File::ReadAll() 
{
	if (!IsExists() || !IsFile()) return "";
	std::ifstream ifs(FileName);
	std::ostringstream oss;
	while ((ifs.peek(), ifs.good())) 
	{
		oss.put((char)ifs.get());
	}
	return oss.str();
}

std::string File::GetFileName()
{
	return FileName;
}

size_t File::Size() 
{
	return FileStatus->st_size;
}

const std::string &File::FullPath() 
{
	return FileName;
}

bool File::IsExists() 
{
	return FileStatus.get() != nullptr;
}

bool File::IsFile() 
{
	// a regular file
	return IsExists() && S_ISREG(FileStatus->st_mode);
}

bool File::IsDirectory() 
{
	return IsExists() && S_ISDIR(FileStatus->st_mode);
}

bool File::IsExecutable() 
{
	return IsExists() && access(FileName.c_str(), X_OK) == 0;
}


bool FileManager::IsStartWithDirectory(const std::string &FileName) 
{
	for (auto ch : FileName)
		if (ch == '/')
			return true;
	return false;
}

FilePtr FileManager::SearchFile(const std::string &FileName) 
{
	auto FileInfo = FilePtr(new File(FileName));
	if (FileInfo->IsExists())
		return FileInfo;

	// search file in path environ variable
	char *PathEnvironment = getenv("PATH");
	std::istringstream ISS(PathEnvironment);

	std::string Path;
	while (std::getline(ISS, Path, ':'))
	{
		std::string FullPath = Path + "/" + FileName;
		auto FileInfo = FilePtr(new File(FullPath));
		if (FileInfo->IsExists())
			return FileInfo;
	}

	return FilePtr(new File());
}

