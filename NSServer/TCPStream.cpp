/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  TCPStream.cpp

  Provide TCP stream.
 ===================================================
  Author: NSKernel
==================================================*/

#include <unistd.h>
#include <cstring>
#include <sstream>
#include <memory>
#include <algorithm>

#include "TCPLayer.h"
#include "Global.h"



TCPBuf::TCPBuf() :
	conn(-1),
	buf(nullptr)
{
}

TCPBuf::TCPBuf(int conn) :
	conn(conn),
	buf(new char[bufsize])
{
	setg(buf + putbacksize,
		buf + putbacksize,
		buf + putbacksize);
}

TCPBuf::~TCPBuf() 
{
	if (conn > 0) 
	{
		std::stringstream LogStream;
		LogStream << "Connection closed from " << conn << ".";
		Logging::Log("STATUS", LogStream.str());
		close(conn);
	}

	if (buf) 
		delete[]buf;
	conn = -1;
}

TCPBuf::TCPBuf(TCPBuf && other) :
	conn(other.conn),
	buf(other.buf)
{
	other.buf = nullptr;
	other.conn = -1;
}

int TCPBuf::overflow(int ch) 
{
	if (ch != EOF) 
	{
		char z = (char)ch;
		if (write(conn, &z, 1) != 1) 
		{
			return EOF;
		}
	}
	return ch;
}

std::streamsize TCPBuf::xsputn(const char *s, std::streamsize n) 
{
	return write(conn, s, n);
}

int TCPBuf::underflow() 
{
	if (gptr() < egptr()) 
	{
		return traits_type::to_int_type(*gptr());
	}

	int num_putback = (int)(gptr() - eback());
	if (num_putback > putbacksize) 
	{
		num_putback = putbacksize;
	}

	std::memmove(buf + (putbacksize - num_putback), gptr() - num_putback, num_putback);

	ssize_t num = read(conn, buf + putbacksize, bufsize - putbacksize);
	if (num <= 0) 
	{
		return EOF; // end or error
	}

	// reset buffer
	setg(buf + (putbacksize - num_putback), buf + putbacksize, buf + putbacksize + num);

	return traits_type::to_int_type(*gptr());
}

TCPBuf::operator bool() 
{
	return conn > 0;
}

TCPStream::TCPStream(int conn) :
	std::iostream(0), tcpbuf(conn)
{
	rdbuf(&tcpbuf);
}

TCPStream::TCPStream(TCPStream &&other) :
	tcpbuf(std::move(other.tcpbuf))
{
}

TCPStream::operator bool() 
{
	return tcpbuf;
}
