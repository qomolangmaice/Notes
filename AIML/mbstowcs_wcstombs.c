/*
  Linux下面的没有命名为 WideCharToMultiByte() 和 MultiByteToWideChar() 函数，
  WideCharToMultiByte，MultiByteToWideChar是windows下的函数，在linux下也有类似的两个函数：

  mbstowcs() 
  wcstombs() 
  
  */

#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

void test0(void)
{

	char buf[256]={"你好啊"};
	setlocale(LC_ALL,"zh_CN.UTF-8");
	wchar_t ar[256]={'\0'};
	int read=mbstowcs(ar,buf,strlen(buf)); 
	printf("%d\n",strlen(buf)); 
	printf("%d\n",read); 
}

void test1(void)
{
	setlocale(LC_ALL,"zh_CN.GB18030");
	wchar_t wcstr[20] = L"字符测试123abc";

	int len = wcslen(wcstr)+1;
	printf("len = %d\n",len);
	for(int i = 0; i < len; i++)
	    printf("0x%08x ",wcstr[i]);
	printf("\n");

	char str[55] = {0};    
	int n = wcstombs(str,wcstr,55);
	if(-1 == n)
	{
		perror("wcstombs ");
		exit(-1);
	}    

	printf("n = %d\n",n);
	for(int i = 0; i < n+1; i++)
	    printf("0x%08x ",str[i]);
	printf("\n");    
	
	wchar_t wch[50]={0};
	int m = mbstowcs(wch,str,n);    
	if(m == -1)
	{
		perror("Converting");
		exit(-1);
	}
	printf("m = %d\n",m);
	for(int i =0; i<m+1;i++)
		printf("0x%08x ",wch[i]);
	printf("\n");
}


int main(void)
{
    test0();
    test1();
    return 0;
}

