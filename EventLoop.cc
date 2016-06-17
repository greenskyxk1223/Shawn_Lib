#include "EventLoop.h"

EventLoop::EventLoop()
{
	// TODO 生成acceptor, epoller
}

void EventLoop::loop()
{
	// 执行epoll_wait();---在此之前应该生成readCallback, writeCallback
}