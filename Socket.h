#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "InetAddr.h"

class Socket
{
public:
	typedef void *(SocketCallback)(int);		//通过这种方式定义原始指针

	Socket(int fd);			// TODO 怎么处理这个fd
	void bindOrDie(const InetAddr& tAddr);
	
	// TODO : not compatible with epoll
	void listenOrDie();

	void shutDownWrite();

	void setReuseAddr(bool on);
	//void setReusePort(bool on);
	void setTcpNoDelay(bool on);

	const int getFd()
	{
		return _iSockfd;
	}
	//int read(void* buf, int len);
	//int write(const void* buf, int len);
	//void setReadCallback();
	//void setWriteCallback();
private:
	int _iSockfd;

	// 这个也不是fd必须的, linux在设置好这些以后, 
	//	会自动把ip+port绑定到指定的fd上, 并自动维持这种联系
	//InetAddr _tInetAddr;		

};

#endif
