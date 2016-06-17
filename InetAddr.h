#ifndef __INETADDR_H__
#define __INETADDR_H__

#include <string>
#include <vector>
#include <netinet/in.h>

class InetAddr
{
public:
	InetAddr(std::string sIp, int iPort);
	//explicit InetAddr(std::string sIpPort);			//127.0.0.1:80
	//explicit InetAddr(sockadd_in tAddr) : _tAddr(tAddr){}
	explicit InetAddr(int iPort, bool bLoopBackOnly = false);		// for listen only
	// TODO : 根据域名获取sockaddr_in
	
	// 获取bind\listen所需要的sockaddr地址
	const struct sockaddr* getSockAddr()
	{
		return static_cast<struct sockaddr*>(&_tAddr);
	}
private:
	struct sockadd_in _tAddr;
};

#endif
