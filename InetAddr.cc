#include "InetAddr.h"

#include <assert.h>
#include <netdb.h>
#include <string.h>
#include <strings.h> // bzero
#include <arpa/inet.h>

InetAddr::InetAddr(std::string sIp, int iPort)
{
	::bzero(&_tAddr, sizeof(_tAddr));
	_tAddr.sin_family = AF_INET;
	_tAddr.sin_port = iPort;
	// TODO
	//_tAddr.sin_addr.s_addr = htonl()
}

InetAddr::InetAddr(int iPort, bool bLoopBackOnly)
{
	::bzero(&_tAddr, sizeof(_tAddr));
	_tAddr.sin_port = htons(iPort);
	_tAddr.sin_family = AF_INET;
	_tAddr.sin_addr.s_addr = htonl(loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY);
}