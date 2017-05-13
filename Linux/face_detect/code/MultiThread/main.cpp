//	compile: g++ -o main main.cpp MultiThread.cpp -lpthread
// 	run: ./main

#include "MultiThread.h"
#include <iostream>

using namespace std;

int main()
{
	pthread_t tid;
	MultiThread mt;

	mt.createThread(tid);
	//mt.joinThread(tid);

	mt.detachThread(tid);

	return 0;
}
