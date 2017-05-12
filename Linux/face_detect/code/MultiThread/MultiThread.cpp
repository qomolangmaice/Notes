#include "MultiThread.h"
#include <string>
#include <pthread.h>

MultiThread::MultiThread()
{
	
}

MultiThread::~MultiThread()
{
	
}

void *threadFunction(void *arg)
{
	MultiThread *multi_thread = (MultiThread *)arg;
	printf("get p:%d\n", multi_thread->p);			// 调用公共成员变量
	multi_thread->sayHello(12);						// 调用公共成员函数
	for(;;);
}

void MultiThread::sayHello(int r)
{
	printf("Hello world %d!\n", r);
}

void MultiThread::createThread()
{
	p = 1;
	q = 2;
	pthread_t threadID;
	pthread_create(&threadID, NULL, threadFunction, this);
	getchar();
}
