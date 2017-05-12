#ifndef MULTI_THREAD_H
#define MULTI_THREAD_H

class MultiThread
{
public:
	MultiThread();
	~MultiThread();

public:
	int p;
	void sayHello(int r);
	void createThread();

private:
	int q;
};

#endif
