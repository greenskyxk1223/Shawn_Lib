#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "Accpter.h"
#include "Epoll.h"
// EventLoop中默认生成Acceptor和Epoller, 同时在这里设置好回调函数
class EventLoop
{
public:
	EventLoop();
	
	void setReadCB(Callback cb)			
	{
		_readCB = cb;
	}
	void setWriteCB(Callback cb)
	{
		_writeCB = cb;
	}

	void loop();

	

private:
	Accepter _tAccepter;

	Epoll _tEpoller;

	Callback _readCB;
	Callback _writeCB;
};


#endif
