//	compile: g++ -o main main.cpp MultiThread.cpp -lpthread
// 	run: ./main

#include "MultiThread.h"
#include <string>
#include <iostream>
#include <map>

using namespace std;

int main()
{
	MultiThread mt;
	mt.createThread();

	return 0;
}
