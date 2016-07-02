#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <string>
// Channel作为基类, 处理普通fd的读写问题------Accepter会覆盖其handleRead()

// TODO 生命周期由谁处理?---使用TcpStreamer进行封装?

// TODO channel同时要处理好state--EPOLLIN, EPOLLOUT等状态
typedef int(*Callback)(std::string&, int fd);		//用户拿到一串字符, 进行处理

class Channel
{
public:
	Channel();
	explicit Channel(int fd);
	virtual ~Channel();		//如果base class没有定义一个析构函数
	virtual void handleRead();		
	void handleWrite();

	void setReadCB(Callback cb)			//允许user设置回调, 便于外部实现
	{
		_readCB = cb;
	}
	void setWriteCB(Callback cb)
	{
		_writeCB = cb;
	}
private:
	Socket _fd;
	std::string sBuffer;

	Callback _readCB;
	Callback _writeCB;
};

#endif