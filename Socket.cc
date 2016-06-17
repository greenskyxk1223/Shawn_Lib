#include "Socket.h"

#include <iostream>

#define MAXCONNECTION 1024

Socket::Socket(int fd) : _iSockfd(fd) {}

void Socket::bindOrDie(const InetAddr& tAddr)
{
	int iRet = ::bind(_iSockfd, tAddr.getSockAddr(), sizeof(struct sockaddr));
	if (iRet)
	{
		std::cout << "Bind error" << std::endl;
		exit(-1);
	}
}

void Socket::listenOrDie()
{
	int iRet = ::listen(_iSockfd, MAXCONNECTION);
	if (iRet)
	{
		std::cout << "listen error" << std::endl;
		exit(-1);
	}
}

void Socket::setReuseAddr(bool on)
{
	int iOption = on ? 1 : 0;			//on为真则为1, 否则为0
	::setsockopt(_iSockfd, SOL_SOCKET, SO_REUSEADDR, &iOption, sizeof(iOption));
}


void Socket::setTcpNoDelay(bool on)
{
	int iOption = on ? 1 : 0;
	::setsockopt(_iSockfd, IPPROTO_TCP, TCP_NODELAY, &iOption, sizeof(iOption));
}
