#ifndef __EPOLL_H__
#define __EPOLL_H__

#include <>

// singleton
class Epoll
{
public:
	// TODO : 为什么这里必须为static, 为什么_tInstance必须为static?
	// 因为静态函数独立于对象存在， 其内部不能存在包含this指针的对象，只能由static对象
	// 静态成员函数属于类本身，而不属于类的对象
	static Epoll& getInstance();
	void addEvent(int iFd, int iState);
	void deleteEvent(int iFd, int iState);
	void modifyEvent(int iFd, int iState);
	void poll();

private:
	// 单例模式意味着class不能被他人创建、复制、拷贝（class of 3)
	Epoll();
	Epoll(const Epoll& rhs);
	Epoll& operator=(const Epoll& rhs);
	int _iEpollFd;
	//static Epoll* _tInstance = NULL;	//在这里初始化？ ----不可以在类的内部初始化非constexpr静态成员
	static Epoll* _tInstance;
	static const int iInitEventListSize = 16;
	
	typedef std::vector<struct epoll_event> vEventArray;

	EventArray _tEvents;

	//TODO : 考虑多线程的情况， 此处应该加锁
};

Epoll::_tInstance = NULL;			//在这里定义?可否?

#endif
