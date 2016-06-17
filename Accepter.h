#ifndef __ACCPTER_H__
#define __ACCPTER_H__

#include "Socket.h"
#include "Channel.h"

class Accepter : public Channel
{
	Accepter(int fd, InetAddr tAddr, bool bReuse = true, bool bNoDelay = false);
	virtual void handleRead();			//接收新连接
	
	void setReadCB(Callback cb)
	{
		_readCB = cb;
	}
	void setWriteCB(Callback cb)
	{
		_writeCB = cb;
	}

	// TODO : accepter每添加一个新连接, 都会保存再epoller的联系表中
private:
	class Epoll;
	Epoll* _tEpoller;		//Accepter属于哪个epoller
	Socket _tListenFd;

	Callback _readCB;		//Accepter直接调用基类的setReadCB, setWriteCB
	Callback _writeCB;
};

#endif
