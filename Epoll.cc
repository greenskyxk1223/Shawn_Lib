#include "Epoll.h"
#include <sys/epoll.h>
#include <iostream>

using namespace std;

Epoll& Epoll::getInstance()
{
	if (_tInstance == NULL)
	{
		_tInstance = new Epoll();
	}
	return *_tInstance;
}

Epoll::Epoll() : _tEvents(16), _iEpollFd(::epoll_create(_tMaxEventsNum))
{}

Epoll::Epoll(const Epoll& rhs)
{}

Epoll& operator=(const Epoll& rhs)
{}

void Epoll::addEvent(int iFd, int iState)
{
	struct epoll_event ev;
	ev.events = iState;
	ev.data.fd = iFd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, iFd, &ev) == -1)
	{
		cout << "epoll_ctl add fd error" << endl;
	}
	cout << "addEvent is ok " << endl;
}

void Epoll::deleteEvent(int iFd, int iState)
{
	struct epoll_event ev;
	ev.events = iState;
	ev.data.fd = iFd;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, iFd, &ev);
}

void Epoll::modifyEvent(int iFd, int iState)
{
	struct epoll_event ev;
	ev.events = iState;
	ev.data.fd = iFd;
	if (epoll_ctl(epollfd, EPOLL_CTL_MOD, iFd, &ev) == -1)
	{
		cout << "epoll_ctl add fd error" << endl;
	}
}

void Epoll::poll()
{
	//int iNums = ::epoll_wait(_iEpollFd, _tEvents, _tEvents.size(), -1);
	int iNumEvents = ::epoll_wait(_iEpollFd, &*(_tEvents.begin()), _tEvents.size(), -1);
	if (static_cast<size_t>(iNumEvents) == _tEvents.size())
	{
		_tEvents.resize(iNumEvents * 2);
	}

	for (int i = 0; i < iNumEvents; ++i)
	{
		int tFd = _tEvents[i].data.fd;
		// FIX �������ȷ��iListenFd�ľ��������
		if (tFd == iListenFd && _tEvents[i].events & EPOLLIN)
		{
			// accpet������, ������epoll_ctl��ӹ�ע��fd
			int iConnFd = ::accept(iListenFd, NULL, NULL);
			if (iConnFd < 0)
			{
				cout << "accept error" << endl;
			}
			else
				addEvent(iConnFd, EPOLLIN);
		}
		else if (_tEvents[i].events & EPOLLIN)
		{
			//TODO : ���Ӧ������socket����д���  do read
			doRead(_tEvents[i].data.fd, buf);
		}
		else if (_tEvents[i].events & EPOLLOUT)
		{
			//TODO : ���Ӧ������socket����д��� do write
			doWrite(_tEvents[i].data.fd, buf);
		}
}