/*
 *  File Name: eeprom_ergodic_test.c
 *  Description: eeprom 遍历测试
 *  Author: 
 *  Created: 
 */
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <linux/rtc.h>
#include <asm-generic/errno-base.h>
#include <linux/capability.h>
#include <termios.h>
#include <sys/ioctl.h>

#define     EEPROM_FILE      "/sys/class/i2c-adapter/i2c-0/0-0050/eeprom"
#define     EEPROM_MAX_SIZE  (32 * 512)
#define     BUF_SIZE         32
#define     ONE_LINE_HEX_LEN 16

typedef unsigned char u8;
typedef unsigned int u32;

static void print_one_line(u8 *buf, int buf_len);
static void print_buf(u32 start_addr, u8* buf, u32 buf_len);
static void write_eeprom(u32 fd, u32 start_addr, u8* buf, u32 buf_len);
static void read_eeprom(u32 fd, u32 start_addr, u8* buf, u32 buf_len);
static void compare_write_and_read(u8* write_buf, u8* read_buf, u32 buf_len, u32 count);

int main(int argc, char* argv[])
{
    u32 i, j, fd;
    u32 start_addr = 0;
    u8 write_buf[BUF_SIZE];
    u8 read_buf[BUF_SIZE];
    
    memset(write_buf, 0, BUF_SIZE);
    memset(read_buf, 0, BUF_SIZE);
    
    fd = open(EEPROM_FILE, O_RDWR);
    if(fd < 0)
    {
        printf("open eeprom failed!\n");
        return 1;
    }

    printf("----------------------------------------------------------\n");
    for (i = 0; i < EEPROM_MAX_SIZE / BUF_SIZE; i++)
    {
        for (j = 0; j < BUF_SIZE; j++)
        {
            write_buf[j] = j + 'A';
        }
        
        start_addr = (unsigned int)(i * BUF_SIZE);
        lseek(fd, start_addr, SEEK_SET);
        
        write_eeprom(fd, start_addr, write_buf, BUF_SIZE);

        usleep(2000);
        
        read_eeprom(fd, start_addr, read_buf, BUF_SIZE);
        
        compare_write_and_read(write_buf, read_buf, BUF_SIZE, i);
        
        printf("----------------------------------------------------------\n"); 
    }
    
    printf("%d x 32 bytes = 16K eeprom. All Done!\n", i);
    
    close(fd);
    return 0;
}

/*******************************************************************************
* Function Name  	: print_one_line
* Description    	: 一行按16个字节打印数据
* Input          	: buf -- 数据帧指针
*                     buf_len -- 数据帧长度
* Output         	: None
* Return         	: None
* Author			: 	
* Others			: None
********************************************************************************/
static void print_one_line(u8 *buf, int buf_len)
{
    int i = 0;
    
    for(i = 0; i < buf_len; i++)
    {
        printf("%02x ", buf[i]);
    }
    
    printf(" |");
    for (i = 0; i < buf_len; i++) 
    {
        if (buf[i] >= 0x20 && buf[i] <= 0x7e) 
        {
            printf("%c", buf[i]);
        } 
        else 
        {
            printf(".");
        }
    }
    printf("|");
}

/*******************************************************************************
* Function Name  	: print_buf
* Description    	: 每行按16个字节打印数据, 最后一行只打印有效数据部分
* Input          	: start_addr -- 打印起始地址
*                     buf -- 数据帧指针
*                     buf_len -- 数据帧长度
* Output         	: None
* Return         	: None
* Author			: 	
* Others			: None
********************************************************************************/
static void print_buf(u32 start_addr, u8* buf, u32 buf_len)
{
    u32 i = 0, j = 0;

	if (buf_len <= ONE_LINE_HEX_LEN)
	{
	    printf("%08X:  ", start_addr);
        print_one_line(buf, buf_len);
	}
	else
	{
	    // 先打印16倍数的数据部分
        for (i = 0; i < buf_len / ONE_LINE_HEX_LEN; i++)
    	{
    	    printf("%08x:  ", start_addr);
    		print_one_line(buf + i * ONE_LINE_HEX_LEN, ONE_LINE_HEX_LEN);
    		start_addr += ONE_LINE_HEX_LEN;
        	printf("\n");
    	}

        // 再打印16余数的数据部分
    	if (buf_len % ONE_LINE_HEX_LEN != 0)
    	{
    	    printf("%08x:  ", start_addr);
    	    print_one_line(buf, buf_len % ONE_LINE_HEX_LEN);
    	}
	}
	printf("\n");
}

/*******************************************************************************
* Function Name  	: write_eeprom
* Description    	: 向eeprom中写入数据
* Input          	: fd -- 待写入数据的eeprom设备节点文件描述符
*                     start_addr -- 写入起始地址
*                     buf -- 数据帧指针
*                     buf_len -- 数据帧长度
* Output         	: None
* Return         	: None
* Author			: 	
* Others			: None
********************************************************************************/
static void write_eeprom(u32 fd, u32 start_addr, u8* buf, u32 buf_len)
{
    u32 ret = 0;
    ret = write(fd, buf, buf_len);
    if(ret < 0)
    {
        printf("write eeprom failed!\n");    
        return ;
    }
    printf("write 32 bytes into eeprom: \n"); 
    print_buf(start_addr, buf, buf_len);
}

/*******************************************************************************
* Function Name  	: read_eeprom
* Description    	: 从eeprom中读取数据
* Input          	: fd -- 读取数据的eeprom设备节点文件描述符
*                     start_addr -- 读取起始地址
*                     buf -- 数据帧指针
*                     buf_len -- 数据帧长度
* Output         	: None
* Return         	: None
* Author			: 	
* Others			: None
********************************************************************************/
static void read_eeprom(u32 fd, u32 start_addr, u8* buf, u32 buf_len)
{
    u32 ret = 0;
    ret = read(fd, buf, buf_len);
    if(ret < 0)
    {
        printf("read eeprom failed!\n");
        return ;
    }
    printf("read 32 bytes from eeprom: \n"); 
    print_buf(start_addr, buf, buf_len);
}

/*******************************************************************************
* Function Name  	: compare_write_and_read
* Description    	: 比较向eeprom中写入的数据和从eeprom中读取的数据是否相同
* Input          	: write_buf -- 向eeprom中写入的数据帧指针
*                     read_buf  -- 从eeprom中读取的数据帧指针
*                     buf_len -- 数据帧长度
*                     count -- 当前比较的计数点
* Output         	: None
* Return         	: None
* Author			: 	
* Others			: None
********************************************************************************/
static void compare_write_and_read(u8* write_buf, u8* read_buf, u32 buf_len, u32 count)
{
    if (memcmp(write_buf, read_buf, buf_len) == 0)
    {
        printf("<%03d> -- write 32 bytes = read 32bytes.\n", count + 1);
    }
    else
    {
        printf("<%03d> -- write 32 bytes != read 32bytes.\n", count + 1);
    }
}
