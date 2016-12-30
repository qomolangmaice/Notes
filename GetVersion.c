/*
 *	FileName：    GetVersion.cpp
 *  Description： 获取IBAT_3.0的App_Public.h中的软件的大版本号和小版本号
 *				        并合并成一个最终软件版本号
 *	Author:		  
 *  Date:		  2016.12.28
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef signed   char      int8;
typedef unsigned char      uint8;

typedef signed   short     int16;
typedef unsigned short     uint16;

typedef signed   long      int32;
typedef unsigned long      uint32;
typedef unsigned long long uint64; 

#define TARGET_STR_LEN 	10
#define BUFFER_SIZE		64

uint32 u32SoftVersion = 0;

uint32 GetVersion(FILE *fp, char *search_str);

int main (int argc, char *argv[])
{
	FILE *pGetVerFile = NULL;
	char *pString[2] = {"#define IOBD_SOFT_VER1 ", "#define IOBD_SOFT_VER2 "};

	uint32 u32BigVersion = 0;
	uint32 u32SmallVersion = 0;

	pGetVerFile = fopen(argv[1], "r");
	if (!pGetVerFile)
	{
		printf("Open File \"%s\" Err.\n", argv[1]);
		return 100;
	}

	u32BigVersion = GetVersion(pGetVerFile, pString[0]);
	u32SmallVersion = GetVersion(pGetVerFile, pString[1]);

	u32SoftVersion =  u32BigVersion * 100 + u32SmallVersion;

	printf("u32SoftVersion: %d\n", u32SoftVersion);

	fclose(pGetVerFile);

	return u32SoftVersion;
}

/*********************************************************************************************
* Function Name  	: GetVersion
* Description    	: 查找文件流中特定字符串后面的数字串，并将该数字串转换成整型值返回
* Input          	: fp -- 输入流路径
*					  search_str -- 需要查找的字符串
* Output         	: None
* Return         	: u32Version -- 版本号
* Author		: 
* Others		: 
*********************************************************************************************/
uint32 GetVersion(FILE *fp, char *search_str)
{
	char *pFindPos = NULL;
	uint32 u32Version = 0;
	uint32 u32VersionLen = 0;

	char u8Buffer[BUFFER_SIZE];
	char u8Target[2 * TARGET_STR_LEN];

	memset(u8Buffer, 0, BUFFER_SIZE);
	memset(u8Target, 0, 2 * TARGET_STR_LEN);

	if (fp == NULL || search_str == NULL)
	{
		printf("Error: File pointer or search_str is NULL");
		return 0;
	}

	u32VersionLen = strlen(search_str);

	while(fgets(u8Buffer, BUFFER_SIZE, fp) != NULL)
	{	
		// 找到文件流中特定字符串的指针位置
		pFindPos = strstr(u8Buffer, search_str);	
		if(pFindPos)
		{
			// 拷贝特定字符串的指针位置末尾后的指定长度的字符串到u8Target缓冲区
			strncpy(u8Target, pFindPos + u32VersionLen, TARGET_STR_LEN);
			u8Target[TARGET_STR_LEN + 1] = '\0';
			// 将字符串中的数字转换成整型值
			u32Version = strtol(u8Target, NULL, 16);
			break;
		}
	}

	return u32Version;
}

