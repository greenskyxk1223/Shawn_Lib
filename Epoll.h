#ifndef __EPOLL_H__
#define __EPOLL_H__

#include <>

// singleton
class Epoll
{
public:
	// TODO : Ϊʲô�������Ϊstatic, Ϊʲô_tInstance����Ϊstatic?
	// ��Ϊ��̬���������ڶ�����ڣ� ���ڲ����ܴ��ڰ���thisָ��Ķ���ֻ����static����
	// ��̬��Ա���������౾������������Ķ���
	static Epoll& getInstance();
	void addEvent(int iFd, int iState);
	void deleteEvent(int iFd, int iState);
	void modifyEvent(int iFd, int iState);
	void poll();

private:
	// ����ģʽ��ζ��class���ܱ����˴��������ơ�������class of 3)
	Epoll();
	Epoll(const Epoll& rhs);
	Epoll& operator=(const Epoll& rhs);
	int _iEpollFd;
	static Epoll* _tInstance = NULL;	//�������ʼ����
	static const int iInitEventListSize = 16;
	
	typedef std::vector<struct epoll_event> vEventArray;

	EventArray _tEvents;

	//TODO : ���Ƕ��̵߳������ �˴�Ӧ�ü���
};

#endif
