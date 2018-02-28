/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  FileSystem.h

  Provide file system support.
 ===================================================
  Author: NSKernel
==================================================*/

#include <string>
#include <memory>
#include <cstdlib>

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

class File 
{
	std::string FileName;
	std::shared_ptr<struct stat> FileStatus;
public:
	File();
	File(const std::string &filename);

	// APIs
	std::string FileExtensionName();
	std::string ReadAll();
	std::string GetFileName();

	size_t Size();

	bool IsExists();
	bool IsFile();
	bool IsDirectory();
	bool IsExecutable();

	const std::string &FullPath();
};

using FilePtr = std::shared_ptr<File>;


class FileManager 
{
public:
	static bool IsStartWithDirectory(const std::string &FileName);
	static FilePtr SearchFile(const std::string &FileName);
};

#endif