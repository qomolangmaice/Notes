/*
 *	FileName：    GetRemoteBin.cpp
 *  Description： 给IBAT_3.0的APP程序的bin文件添加文件头，
 *				        文件头中包含了APP程序的bin文件的CRC16校验值
 *	Author:	
 *  Date:		  2016.12.29
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define IH_NMLEN			32
#define IH_MAGIC			0x12345678
#define IH_IMG_BIOS			0x00
#define IH_IMG_APP			0x01
#define BOARD_TYPE			0x00
#define SOFTWARE_ID			0x01

#define CRC_FLAG_RESTART			0	// CRC - 重启标志
#define CRC_FLAG_CAL_BIT_TORRENT	1	// CRC - 计算比特流标志
#define CRC_FLAG_GET_HIGH			2	// CRC - 获取高位CRC数据标志
#define CRC_FLAG_GET_LOW			3	// CRC - 获取地位CRC数据标志

#define READ_BUF_SIZE		256

typedef signed   char      int8;
typedef unsigned char      uint8;

typedef signed   short     int16;
typedef unsigned short     uint16;

typedef signed   long      int32;
typedef unsigned long      uint32;
typedef unsigned long long uint64; 

typedef volatile unsigned long  vu32;

typedef unsigned char      u8;

typedef unsigned short     u16;

typedef unsigned long      u32;
typedef signed long		   s32;
typedef signed short	   s16;
typedef signed char		   s8;

union unCrc
{
    uint16 u16CRCData;
    uint8 u8CRCData[2];	// Low:  u8CRCData[0] 
						// High: u8CRCData[1]
};

union unCrc crc;	// 存储app_bin的CRC16校验值的联合体
 
uint16 const s_au16CrcTable[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

typedef struct UPDATE_FILE_HEAD{
	unsigned char m_ucImageName[IH_NMLEN];	// Image Name 
	unsigned int m_uiMagicNum;				// Image Header Magic Number 
	unsigned int m_uiSize;					// Image Data Size, not include image header
	unsigned int m_uiLoadAddr;				// Data Load Address 
	unsigned short m_usDataCrc;				// Image Data CRC Checksum, not include image header
	unsigned short m_usImageVer;			// Image version 
	unsigned char m_ucCpuArch;				// CPU architecture 
	unsigned char m_ucBoardType;			// Board type 
	unsigned char m_ucManufacturer;			// manufacturer 
	unsigned char m_ucImageType;			// Image type 
	unsigned char m_ucBootType;				// boot type 
	unsigned char m_ucReserved1[64];		// Reserve for fucture use
	unsigned char m_ucSoftwareId;			// Software ID
	unsigned char m_ucReserved2[10];		// Reserve for fucture use
}UPDATE_FILE_HEAD_t;

uint8 CalcCrc16(uint8 CalMode, uint8* Packet, int dwLength);

int main (int argc, char *argv[])
{
	FILE* app_bin;
	FILE* app_with_file_header_bin;

	uint8 u8Buf[READ_BUF_SIZE];
	uint32 u32ReadLen = 0;
	uint32 u32WriteLen = 0;
	uint32 u32AppendLen = 0;
	uint32 u32CountLen= 0;
	UPDATE_FILE_HEAD_t pImage_Header = {0};

	app_bin = fopen(argv[1], "rb" );
	app_with_file_header_bin  = fopen(argv[2], "wb" );

	if (!app_bin)
	{
		printf("Cannot open app_bin file.\n");
		exit(0);
	}

	// 缓冲区清零
	memset(u8Buf, 0, READ_BUF_SIZE);
	// 1. 计算app_bin文件长度及CRC 
	do
	{
		u32ReadLen = fread(u8Buf, 1, 128, app_bin);
		if (u32ReadLen > 0) 
		{
			(void)CalcCrc16(CRC_FLAG_CAL_BIT_TORRENT, u8Buf, u32ReadLen);
			u32CountLen += u32ReadLen;
		}
		else 
		{
			break;
		}
	} while(1);

	// 2. 文件头信息填充
	if (strlen(argv[3]) <= IH_NMLEN)
	{
		// 读取文件名输入参数
		memcpy(pImage_Header.m_ucImageName, argv[3], strlen(argv[3]));
	}
	else
	{
		memcpy(pImage_Header.m_ucImageName, argv[3], IH_NMLEN);
	}
	memset(&pImage_Header.m_ucImageName[strlen(argv[3])], 0xFF, IH_NMLEN - strlen(argv[3]));
	// 缓冲区清零
	memset(u8Buf, 0, READ_BUF_SIZE);
	pImage_Header.m_uiMagicNum		= IH_MAGIC;
	pImage_Header.m_uiSize			= u32CountLen;
	pImage_Header.m_uiLoadAddr		= 0xFFFFFFFF;
	pImage_Header.m_usDataCrc		= (uint16)CalcCrc16(CRC_FLAG_GET_LOW, u8Buf, u32ReadLen);			// 获取crc低位，即crc.u8CRCData[0]
	pImage_Header.m_usDataCrc		|= (uint16)(CalcCrc16(CRC_FLAG_GET_HIGH, u8Buf, u32ReadLen) << 8);	// 获取crc高位，即crc.u8CRCData[1]
	pImage_Header.m_usImageVer		= 0xFFFF;
	pImage_Header.m_ucCpuArch		= 0xFF;
	pImage_Header.m_ucBoardType		= BOARD_TYPE;
	pImage_Header.m_ucManufacturer	= 0xFF;
	pImage_Header.m_ucImageType		= IH_IMG_APP;
	pImage_Header.m_ucBootType		= atoi(argv[4]);
	memset(pImage_Header.m_ucReserved1, 0xFF, 64);
	pImage_Header.m_ucSoftwareId  = SOFTWARE_ID;
	memset(pImage_Header.m_ucReserved2, 0xFF, 10);

	// 3. 将文件头数据写入app_with_file_header_bin文件
	u32WriteLen = fwrite((char *)&pImage_Header, 1, 128, app_with_file_header_bin);
	if (128 != u32WriteLen)
	{
		printf("write head info err.\n");
	}

	// 缓冲区清零
	memset(u8Buf, 0, READ_BUF_SIZE);
	// 4. 将app_bin文件写入app_with_file_header_bin文件
	fseek(app_bin, 0, SEEK_SET);
	do
	{
		u32ReadLen = fread(u8Buf, 1, 16, app_bin);
		if (u32ReadLen > 0)
		{
			fwrite(u8Buf, 1, u32ReadLen, app_with_file_header_bin); 
		}
		else 
		{
			break;
		}
	} while(1);

	fclose(app_bin);
	fclose(app_with_file_header_bin);

    return 0;
}

/*********************************************************************************************
* Function Name  	: CalcCrc16
* Description    	: 计算CRC16校验字值
* Input          	: CalMode -- 计算模式
*					          Packet --  计算CRC的比特流
*					          dwLength -- 比特流长度
* Output         	: None
* Return         	: 返回 crc.u8CRCData[0] -- CRC16校验值低位
*					           或者返回 crc.u8CRCData[1] -- CRC16校验值高位
* Author			    : 
* Others			    : 该函数的主要处理为查CRC16计算表以便得到CRC16校验字。该计算表是由
*					         另外一个按照CRC-16标准的计算方法和种子数的函数计算得出。
*********************************************************************************************/
uint8 CalcCrc16(uint8 CalMode, uint8* Packet, int dwLength)
{
	int32 i;

	switch (CalMode)
	{
	case CRC_FLAG_RESTART:
		crc.u16CRCData = 0x0000;
		break;

	case CRC_FLAG_CAL_BIT_TORRENT:
		for (i = 0; i < dwLength; i++)
		{
			crc.u16CRCData = ((crc.u16CRCData << 8) | Packet[i]) ^ s_au16CrcTable[(crc.u16CRCData >> 8) & 0xFF];
		}
		for (i = 0; i < 2; i++)
		{
			crc.u16CRCData = ((crc.u16CRCData << 8) | 0) ^ s_au16CrcTable[(crc.u16CRCData >> 8) & 0xFF];
		}
		break;

	case CRC_FLAG_GET_HIGH:
		return crc.u8CRCData[1];

	case CRC_FLAG_GET_LOW:
		return crc.u8CRCData[0];

	default:
		break;
	}
	return 0;
}
