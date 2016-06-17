#include "Accpter.h"
#include "Epoll.h"

Accepter::Accepter(int fd, InetAddr tAddr, bool bReuse, bool bNoDelay) : _tListenFd(fd)
{
	_tListenFd.bindOrDie(tAddr);
	_tListenFd.listenOrDie();
	_tListenFd.setReuseAddr(bReuse);
	_tListenFd.setTcpNoDelay(bNoDelay);
}

void Accepter::handleRead()
{
	int iFd = ::accept(_tListenFd, 0, 0);		//TODO : 实现保存后面client的地址
	Channel tChannel(iFd);

	tChannel.setReadCB(_readCB);
	tChannel.setWriteCB(_writeCB);
	// TODO: epoller需要维护一个map<fd, TcpStreamer>来管理这些TcpConnection
	_tEpoller->addEvent(tChannel);		// 默认传入POLLIN, POLLOUT的状态
	
	
}
