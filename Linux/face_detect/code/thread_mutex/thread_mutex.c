/*
 *     File Name: mutex_num.c
 *     Description: 
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2016.09.03 17:10:21
 */
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

#include "print.h"

int lock_val; 	// 要保护的资源
time_t end_time;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void pthread1(void *arg);
void pthread2(void *arg);

int main(int argc, char *argv[])
{
	pthread_t td1, td2;
	int ret;

	end_time = time(NULL) + 30;

	pthread_mutex_init(&mutex, NULL);

	ret = pthread_create(&td1, NULL, (void *)pthread1, NULL);
	if (ret != 0)
		printf("pthread cread1");

	ret = pthread_create(&td2, NULL, (void *)pthread2, NULL);
	if( ret != 0 )
		printf("pthread cread2");

	pthread_join(td1, NULL);
	pthread_join(td2, NULL);

	exit(0);

    return 0;
}

void pthread1(void *arg)
{
	int i;
	while(time(NULL) < end_time) {
		printf(LIGHT_BLUE"pthread1: before pthread_mutex_lock\n"NONE);

		pthread_mutex_lock(&mutex);
		printf(LIGHT_BLUE"pthread1: after pthread_mutex_lock\n"NONE);
		for( i = 0 ; i < 2 ; i++ )
		{
			sleep(1);
			lock_val++;
			printf(LIGHT_BLUE"lock_var = %d\n"NONE, lock_val);
		}

		printf(LIGHT_BLUE"pthread1: locak_var = %d\n"NONE, lock_val);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

void pthread2(void *arg)
{
	while( time(NULL) < end_time )
	{
		printf(LIGHT_RED"pthread2: before pthread_mutex_lock\n"NONE);
		pthread_mutex_lock(&mutex);
		printf(LIGHT_RED"pthread2: Got lock --> lock_val = %d\n"NONE, lock_val);
		pthread_mutex_unlock(&mutex);
		printf(LIGHT_RED"pthread2: after pthread_mutex_lock\n"NONE);

		usleep(200000); // sleep 0.2s
	}
}

