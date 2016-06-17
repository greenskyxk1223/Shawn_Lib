#include "Channel.h"
#include <iostream>

Channel::Channel(int fd) : _fd(fd) {}

void Channel::handleRead()
{
	//char buf[512];
	// TODO : read()---EOF
	//::read(fd, buf, 512);

	_readCB(sBuffer, _fd);
	std::cout << "fd = " << _fd.getFd() << " read " << sBuffer.size() << " bytes data" << std::endl;
}

// 这里的doRead函数作为一个参考
void doRead(std::string& sBuf, int fd)
{
	char buf[1024];
	::bzero(buf, 1024);
	while (::read(fd, buf, 1024) != EOF)
	{
		sBuf += buf;
		::bzero(buf, 1024);
	}

}

void Channel::handleWrite()
{
	int iRet = _writeCB(sBuffer, _fd);		// todo, 这里读了多少数据啊?

	std::cout << "fd = " << _fd.getFd() << " write " << iRet << " bytes data" << std::endl;
}