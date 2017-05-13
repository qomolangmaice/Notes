#include "MultiThread.h"
#include <iostream>
#include <ctime>

using namespace std;

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

	//cout << "Hello world " << r << "!" << end;
}

void MultiThread::createThread(pthread_t tid)
{
	p = 1;
	q = 2;
	pthread_create(&tid, NULL, threadFunction, this);
	//getchar();
}

void MultiThread::joinThread(pthread_t tid)
{
	pthread_join(tid, NULL);
	cout << "After pthread_join.\n" << endl;
}

void MultiThread::detachThread(pthread_t tid)
{
	cout << "Before pthread_detach.\n" << endl;
	pthread_detach(tid);
	cout << "After pthread_detach.\n" << endl;
}
