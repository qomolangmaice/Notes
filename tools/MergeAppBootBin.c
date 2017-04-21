/*
 *	FileName：    MergeAppBootBin.c
 *  Description： 合并IBAT_3.0的App和Boot的bin文件
 *				  
 *	Author:		  
 *  Date:		  2016.12.29
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

#define BUF_SIZE			128
#define RESERVED_FRAME_SIZE	(272 * 1024 / 128)		// 272k预留的Flash空间

uint32 WriteBinToFile(FILE* src_path, FILE** dst_path);
void MergeFile(char *app_bin_path, char *boot_bin_path, char *dst_path);

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("Usage: MergeAppBootBin.exe ENT1DETB.bin Boot.bin ENT1DETB501.bin\n");
		system("pause");
		return -1;
	}

	MergeFile(argv[1], argv[2], argv[3]);

	return 0;
}

/*******************************************************************************
* Function Name  	: WriteBinToFile
* Description    	: 将bin文件内容写入到一个新的bin文件中
* Input          	: src_path -- 输入流路径
*					          dst_path -- 输出流路径指针
* Output         	: None
* Return         	: u32Count -- 返回写入的总字节数
* Author			    :
* Others			    : 
********************************************************************************/
uint32 WriteBinToFile(FILE* src_path, FILE** dst_path)
{
	uint8 u8Buf[BUF_SIZE];
	uint32 u32Count = 0;
	uint32 u32ReadNum = 0;

	if (src_path == NULL | dst_path == NULL)
	{
		printf("open file error!\n");
		return 0;
	}

	memset(u8Buf, 0, BUF_SIZE);

	do 
	{
		u32ReadNum = fread(u8Buf, 1, 128, src_path);
		if (u32ReadNum > 0)
		{
			fwrite(u8Buf, 1, 128, *dst_path);
			u32Count += u32ReadNum;
		}
		else
			break;
	} while (1);

	return u32Count;
}

/*******************************************************************************
* Function Name  	: MergeFile
* Description    	: 将IBAT_3.0的App和Boot的bin文件合并一个新的bin文件中
* Input          	: app_bin_path -- app_bin输入流路径
*					  boot_bin_path -- boot_bin输入流路径
*					  dst_path -- 输出流路径
* Output         	: None
* Return         	: NONE
* Author			: wangkeguan wwx423495
* Others			: 
********************************************************************************/
void MergeFile(char *app_bin_path, char *boot_bin_path, char *dst_path)
{
	uint8 u8Buffer[BUF_SIZE];

	// 1. 打开app_bin文件和boot_bin文件，写入一个空的app_boot_bin文件
	FILE* app_bin = fopen(app_bin_path, "rb");
	FILE* boot_bin = fopen(boot_bin_path, "rb");
	FILE* app_boot_bin = fopen(dst_path, "wb");

	if (app_bin == NULL || boot_bin == NULL)
	{
		printf("Can't open file!\n");
		return;
	}

	// 2. 将app_bin文件写入到app_boot_bin中
	WriteBinToFile(app_bin, &app_boot_bin);

	// 3. 272k的预留Flash全部填0xFF
	memset(u8Buffer, 0xFF, BUF_SIZE);
	for (int i = 0; i < RESERVED_FRAME_SIZE; i++)
	{
		fwrite(u8Buffer, 1, 128, app_boot_bin);
	}
	
	// 4. 将boot_bin文件写入到app_boot_bin中
	WriteBinToFile(boot_bin, &app_boot_bin);

	fclose(app_bin);
	fclose(boot_bin);
	fclose(app_boot_bin);
}
