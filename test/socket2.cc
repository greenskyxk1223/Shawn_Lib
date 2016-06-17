#include <iostream>
#include <fcntl.h>			// 对fd的操作--open\read\write\close
//#include <sys/stat.h>
//#include <unistd.h>		// read
#include <string.h>
// socket相关
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <unistd.h>
// epoll related
#include <sys/epoll.h>

#include <stdlib.h>		//exit头文件
#include "/home/tafjce/Comm/Util/json/json.h"    // json parse file
	
using namespace std;

string& getFileContent()
{
	int iFd = ::open("./FeesTransferViewController.json", O_RDONLY);
	string sContent;
	{
		char buf[20];
		memset(buf, 0, sizeof(buf));

		while (read(iFd, buf, 20) != 0)
		{
			sContent.append(buf);
 			memset(buf, 0, sizeof(buf));
		}

	}

	cout << "the sContent is \n" << sContent << endl;

	::close(iFd);
	
	return sContent;
}

typedef struct sockaddr SA;

const SA* sockadd_cast(const struct sockaddr_in* addr)
{
	return static_cast<const SA*>((const void*)(addr));
}

void addEvent(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		cout << "epoll_ctl add fd error" << endl;
		exit(-1);
	}
	cout << "addEvent is ok " << endl;
}

void deleteEvent(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

void modifyEvent(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

//重写doRead和doWirte
void doRead(int epollfd, int fd, char* buf)
{
	memset(buf, 0, 1024);
	int iReadNums = read(fd, buf, 1024);
	if (iReadNums == -1)
	{
		cout << "read error" << endl;
		close(fd);
		deleteEvent(epollfd, fd, EPOLLIN);
	}
	else if (iReadNums == 0)	//EOF
	{
		cout << "client is close";
		close(fd);
		deleteEvent(epollfd, fd, EPOLLIN);
	}
	else
	{
		cout << "read message is " << buf << endl;
		// TODO 目前的系统在这里才修改文件描述符可写
		modifyEvent(epollfd, fd, EPOLLOUT);
		
		memset(buf, 0, 1024);
	}
}

void doWrite(int epollfd, int fd, char* buf)
{
	// TODO 这里没有使用buf, 而是采用固定的输入
	string sFile = readFile();
	int iNums = write(fd, sFile.c_str(), sFile.size());
	if (iNums == -1)
	{
		cout << "doWrite error" << endl;
		close(fd);
		deleteEvent(epollfd, fd, EPOLLOUT);
	}
	else
		modifyEvent(epollfd, fd, EPOLLIN);
}

int main()
{
	// ingnore SIGPIPE
	signal(SIGPIPE, SIG_IGN);
	
	char buf[1024];
	memset(buf, 0, 1024);
	
	//socket创建工作
	int iListenFd = ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
	
	sockaddr_in tServerAddr;
	memset(&tServerAddr, 0, sizeof(tServerAddr));
	tServerAddr.sin_family = AF_INET;
	tServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	tServerAddr.sin_port = htons(15364);

	// setReuseAddr
	int on = 1;
	::setsockopt(iListenFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	int iRet = ::bind(iListenFd, sockadd_cast(&tServerAddr), sizeof(tServerAddr));
	if (iRet)
	{
		cout << "Bind error" << endl;
		return -1;
	}

	iRet = ::listen(iListenFd, 5);
	if (iRet)
	{
		cout << "listen error" << endl;
		return -1;
	}

	int tEpollfd = epoll_create(10);
	if (tEpollfd == -1)
	{
		cout << "epoll_create error" << endl;
		return -1;
	}

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = iListenFd;
	iRet = epoll_ctl(tEpollfd, EPOLL_CTL_ADD, iListenFd, &ev);
	if (iRet == -1)
	{
		cout << "epoll add events error" << endl;
		return -1;
	}

	// 用于存放关注事件的结构体
	struct epoll_event tEvents[100];

	while (1)
	{
		int tNums = epoll_wait(tEpollfd, tEvents, 100, -1);
		if (tNums == -1)
		{
			cout << "epoll_wait error " << endl;
			return -1;
		}

		for (int i = 0; i < tNums; ++i)
		{
			int tFd = tEvents[i].data.fd;
			if (tFd == iListenFd && tEvents[i].events & EPOLLIN)
			{
				// accpet新连接, 并调用epoll_ctl添加关注的fd
				int iConnFd = ::accept(iListenFd, NULL, NULL);
				if (iConnFd < 0)
				{
					cout << "accept error" << endl;
					return -1;
				}

				addEvent(tEpollfd, iConnFd, EPOLLIN);
			}
			else if (tEvents[i].events & EPOLLIN)
			{
				//do read
				doRead(tEpollfd, tEvents[i].data.fd, buf);
			}
			else if (tEvents[i].events & EPOLLOUT)
			{
				//do write
				doWrite(tEpollfd, tEvents[i].data.fd, buf);
			}

		}

	}

	return 0;
}