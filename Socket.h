#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "InetAddr.h"

class Socket
{
public:
	Socket();
	void bindOrDie(InetAddr& tAddr);
	// TODO : not compatible with epoll
	void listenOrDie();
private:
	int _sockfd;
	InetAddr tInetAddr;
};

#endif
