/*==================================================
  NServer
  Copyright (C) 2018 NSKernel. All rights reserved.
 ===================================================
  Licensed under GPL v3.
 ===================================================
  TCPLayer.h

  Provide TCP stream and server.
 ===================================================
  Author: NSKernel
==================================================*/

#ifndef TCPLAYER_H
#define TCPLAYER_H

#include <iostream>
#include <streambuf>


class TCPBuf : public std::streambuf {
	int conn;

	static constexpr std::streamsize bufsize = 1024;
	static constexpr std::streamsize putbacksize = 4;
	char *buf;
protected:
	int overflow(int ch) override;
	std::streamsize xsputn(const char *s, std::streamsize n) override;

	int underflow() override;

public:
	TCPBuf();
	TCPBuf(int _conn);
	~TCPBuf();

	TCPBuf(const TCPBuf &) = delete;
	TCPBuf& operator= (const TCPBuf &) = delete;

	TCPBuf(TCPBuf &&);
	TCPBuf& operator= (TCPBuf &&) = delete;

	operator bool();
};


class TCPStream : public std::iostream {
	TCPBuf tcpbuf;
public:
	TCPStream() = default;
	TCPStream(int conn);
	virtual ~TCPStream() = default;

	TCPStream(const TCPStream &) = delete;
	TCPStream& operator= (const TCPStream &) = delete;

	TCPStream(TCPStream &&);
	TCPStream& operator= (TCPStream &&) = delete;

	operator bool();
};

class TCPServer {
protected:
	int Port;
	int ServerFileDescriptor;

private:
	void InitializeServerFileDescriptor();

public:
	TCPServer(int port = 80); // default to be 80 port
	~TCPServer();

	void InitializeServerFileDescriptor(int Port);
	int AcceptedClient();
};

#endif