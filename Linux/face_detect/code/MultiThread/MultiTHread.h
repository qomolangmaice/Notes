#ifndef MULTI_THREAD_H
#define MULTI_THREAD_H

#include <pthread.h>
#include <iostream>

class MultiThread
{
public:
	MultiThread();
	~MultiThread();

public:
	int p;

	void createThread(pthread_t tid);
	void joinThread(pthread_t tid);
	void detachThread(pthread_t tid);

	void sayHello(int r);

private:
	int q;
};

#endif
