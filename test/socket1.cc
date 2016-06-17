#include <iostream>
#include <fcntl.h>			// 对fd的操作--open\read\write\close
//#include <sys/stat.h>
#include <unistd.h>		// read
#include <string.h>

#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

string readFile()
{
	int iFd = open("./xk.txt", O_RDONLY);
	string sContent;
	{
		char buf[20];
		memset(buf, 0, sizeof(buf));

		// read会在内部保持一个index做为已经读取的标识
		// 每次读满20, sContent.append() 是否能够发现?(buf已经被20字节填满,能否发现\0?)
		// 这里内核做了操作, 如果buf被读满了, 内核会在其后面添加上"l\0"这两个字符
		// 但是实际读取中, 该数据会被忽略, 下面index的值变为22
		while (read(iFd, buf, 20) != 0)
		{
			//cout << buf << endl;
			//int index = 0;
			//for (char* p = buf; *p != '\0'; ++p)
			//	++index;
			//cout << "index = " << index << endl;
			//cout << endl;
			//buf[19] = 'x';
			sContent.append(buf);
			//cout << "sContent = " << sContent <<  endl;
 			memset(buf, 0, sizeof(buf));
		}

	}

	//cout << "the buff is all read" << endl;
	cout << "the sContent is \n" << sContent << endl;

	close(iFd);
	
	return sContent;
}

typedef struct sockaddr SA;

const SA* sockadd_cast(const struct sockaddr_in* addr)
{
	return static_cast<const SA*>((const void*)(addr));
}

int main()
{
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
	//cout << "bind ok" << endl;

	iRet = ::listen(iListenFd, 5);			//一般认为(5*影响因子), 是未连接队列+已连接队列的最大数目
	if (iRet)
	{
		cout << "listen error" << endl;
		return -1;
	}
	//cout << "listen ok" << endl;

	while (1)
	{
		// accept(iListenFd,sockadd_cast(&tClientAddr), sizeof(tClientAddr))
		// tClientAddr可以保存客户的信息, 但是用NULL的话, 就不用保存任何信息
		int tConnectFd = ::accept(iListenFd, NULL, NULL);
		if (iRet < 0)
		{
			cout << "accept error" << endl;
			return -1;
		}

		cout << "the connection is accept" << endl;

		string sStr = readFile();
		if (sStr.size() != write(tConnectFd, sStr.c_str(), sStr.size()))
		{
			cout << "write error" << endl;
		}
		cout << "send " << sStr.size() << " bytes" << endl;

		close(tConnectFd);
	}

	return 0;
}