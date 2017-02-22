/****************************************************************************
Copyright: 
File name: 	Xmodem.c
Author: 		
Version: 		
Description:  	实现XMODEM加载功能
Others: 		无
History:
****************************************************************************/
/*********************  系统级别的相关头文件    ***************/

/*********************  bsp提供的相关头文件    ***************/
#include "watchdog.h"

/*********************  内部声明的相关头文件    ***************/
//#include "BspConfig.h"
#include "DriverPublic.h"
#include "InnerUart.h"
#include "InnerFlash.h"
#include "Xmodem.h"

/*********************  相关的宏定义		      ***************/

SerialInput g_stRecCom; /* storage for receiving buffer */

u8 g_au8RecDataBuf[BUF_SIZE];

u16 const s_au16CrcTable[256] = {
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
static u8 CheckSart(u32 * DataNum);
static u8 CheckSeq(u8 u8PackID);
static int XmodemPutChar(int c);
static void WaitForPutChar(void);

/*******************************************************************************
* Function Name  	: CancelXModem
* Description    	: 取消XMODEM模式
* Input          	: None
* Output         	: None
* Return         	: None
* Author			: 	
* Others			: None
********************************************************************************/
void CancelXModem(void)
{
    (void)XmodemPutChar(XMODEM_CAN);
    (void)XmodemPutChar(XMODEM_CAN);
}

/*******************************************************************************
* Function Name  	: WaitForPutChar
* Description    	: 等待字符输出
* Input          	: None
* Output         	: None
* Return         	: None
* Author			: 	
* Others			: None
********************************************************************************/
void WaitForPutChar(void)
{
	u8 u8Rec=0;
	u16 u16DeyTime = 0;

	//while(HWREG(HAL_UART_PORT + UART_O_FR) & UART_FR_BUSY)
	while(HW_REG(HAL_UART_PORT + UART_O_FR) & UART_FR_TXFF)
	{
		WatchdogClear();
		u16DeyTime++;
		if(0xffff == u16DeyTime)
		{
			u16DeyTime = 0;
			u8Rec++;
			if(u8Rec > 40)
			{
				return; /*ero*/
			}
		}
	}
}

/*******************************************************************************
* Function Name  	: XmodemWaitChar
* Description    	: Xmodem模式等待接收字符
* Input          	: None
* Output         	: None
* Return         	: 成功返回接收字符； 失败返回错误码0xff
* Author			: 	
* Others			: None
********************************************************************************/
u8 XmodemWaitChar(void)
{
    u8 u8Rec=0;
    u16 u16DeyTime = 0;

	while(HW_REG(HAL_UART_PORT + UART_O_FR) & UART_FR_RXFE)
    {
        WatchdogClear();
        u16DeyTime++;
        if(0xffff == u16DeyTime)
        {
            u8Rec++;
            if(u8Rec > 40)
            {
                /*超时取消传输*/
                CancelXModem();
                DRV_Printf("\r\nRec Timeout!! break\r\n");
                return 0xff; /*ero*/
            }
        }
    }
	u8Rec = HW_REG(HAL_UART_PORT + UART_O_DR);
	//DRV_Printf("%x ", u8Rec);

    return u8Rec;
}

/*******************************************************************************
* Function Name  	: CalcCrc16
* Description    	: 计算CRC16校验字
* Input          	: Packet   -- 包地址
					  dwLength -- 包长度
* Output         	: None
* Return         	: 成功返回CRC16校验字 
					  错误返回0		
* Author			: 马剑涛	
* Others			: 该函数的主要处理为查CRC16计算表以便得到CRC16校验字。该计算表是由
					  另外一个按照CRC-16标准的计算方法和种子数的函数计算得出，
					  这两个函数已经充分验证，保证其正确性和符合CRC-16的标准。

* Modify record		： <修改人>	   <修改时间>  <问题单号> <修改描述>
					  马剑涛/25593 2004年2月4日 Create First Version.
					  马剑涛/25593 2004年4月6日 修改种子数,更新计算表
********************************************************************************/
u16 CalcCrc16( u8 * Packet, u32 dwLength )
{
	u16 u16Crc16 = 0;
    u32 i;

    for ( i = 0; i < dwLength; i++ )
    {
        u16Crc16 = ((u16Crc16<<8)|Packet[i])^s_au16CrcTable[(u16Crc16>>8)&0xFF];
    }

    for ( i = 0; i < 2; i++ )
    {
        u16Crc16 = ((u16Crc16<<8)|0)^s_au16CrcTable[(u16Crc16>>8)&0xFF];
    }

    return (u16Crc16);
}

/*******************************************************************************
* Function Name  	: CheckSart
* Description    	: 检查sart
* Input          	: None
* Output         	: DataNum	-- Xmodem数据计数
* Return         	: 成功返回Xmodem错误控制字符； 
					  错误返回0
* Author			: 	
* Others			: None
********************************************************************************/
u8 CheckSart(u32 * DataNum)
{
    u8 u8CheckChar = 0;

    u8CheckChar = XmodemWaitChar();
    switch(u8CheckChar)
    {
    case XMODEM_SOH:
        *DataNum = 128;
        return XMODEM_START_OK;

    case XMODEM_STX:
        *DataNum = 1024;
        return XMODEM_START_OK;

    case XMODEM_EOT:
        (void)XmodemPutChar(XMODEM_ACK); //通知PC机全部收到
        return XMODEM_END_EOF;

    case XMODEM_CAN:
        CancelXModem();
        DRV_Printf("\r\nRemot Cancel !!");
        return XMODEM_REMOT_CANCEL;

    default:
        CancelXModem();
        DRV_Printf("\nCheckChar = %X\n", u8CheckChar);
        return XMODEM_START_ERO;
    }
}

/*******************************************************************************
* Function Name  	: CheckSeq
* Description    	: 检查Seq
* Input          	: u8PackID	-- 打包数据的ID
* Output         	: None
* Return         	: Xmodem错误控制字符
* Author			: 	
* Others			: None
********************************************************************************/
u8 CheckSeq(u8 u8PackID)
{
    u8 u8CheckedPID;
    u8 u8CheckingPID;

    u8CheckedPID = XmodemWaitChar();
    u8CheckingPID = XmodemWaitChar();

    u8CheckingPID = ~u8CheckingPID ;
    if( u8CheckedPID == u8CheckingPID )
    {
        if(u8CheckedPID == u8PackID)
        {
            return XMODEM_SEQ_OK;
        }
        else
        {
            /*06/04/20 bug: u8PackID = CheckSeq+1 -> u8PackID == CheckPID+1*/
            if(u8PackID == u8CheckedPID+1)
            {
                return XMODEM_SEQ_REPEAT;
            }
            else
            {
                return XMODEM_SEQ_ERO;
            }
        }
    }
    else
    {
        return XMODEM_SEQ_ERO;
    }
}

/*******************************************************************************
* Function Name  	: WaitXmodem
* Description    	: 等待Xmodem模式选择，如果被选择，则进行Xmodem模式文件加载
* Input          	: None
* Output         	: None
* Return         	: Xmodem错误控制字符
* Author			: 	
* Others			: None
********************************************************************************/
u8 WaitXmodem (void)
{
	u32 u32DelayTime = 0;
	u32 u32PackNo = 1;
	u32 u32CharCount = 0;
	u16 u16Temp = 0;
	u32 u32AppAddress = APP1_START_ADDR;
	u32 u32RecDataNum = 0;
	u8  u8XmodemReturn = 0;

	union
	{
		u16 TWord;
		struct{u8 lo;u8 hi;}DDWord;
	}unCrc;

	//每秒向PC机发送一个控制字符"C"，等待控制字〈soh〉
	while(0 == u32RecDataNum)
	{
		if((HW_REG(HAL_UART_PORT + UART_O_FR) & UART_FR_RXFE) == UART_FR_RXFE)
		{
			u32DelayTime++;
			if(u32DelayTime > 0xffff0)//timer0 over flow
			{
				u32DelayTime = 0;
				WatchdogClear();
				(void)XmodemPutChar(XMODEM_RECIEVING_WAIT_CHAR); //file://send a "C"
				/*超时保护*/
				if ((u16Temp++) > 300)
				{
					return XMODEM_REMOT_CANCEL;
				}
			}
		}
		else
		{
			u8XmodemReturn = HW_REG(HAL_UART_PORT + UART_O_DR);//USART1->DR;
			switch(u8XmodemReturn)
			{
				case XMODEM_SOH:
				u32RecDataNum = 128;
				break;

				case XMODEM_STX:
				u32RecDataNum = 1024;
				break;

				case 'r':
				case 'R':
				DRV_Printf("\r\nReset!\r\n");
				while(1)
				{}

				default:
				CancelXModem();
				DRV_Printf("\r\nERO Modem!Please Change!\r\n");
			}
		}
	}

	u32PackNo = 1;
	//开始接收数据块
	do
	{
		if (( u32DelayTime = CheckSeq(u32PackNo)) == XMODEM_SEQ_OK )
		{
			//file://核对数据块编号正确
			for(u32CharCount = 0; u32CharCount < u32RecDataNum; u32CharCount++)//file://接收128个字节数据
			{
				g_au8RecDataBuf[u32CharCount] = XmodemWaitChar();
			}
			
			unCrc.TWord = 0;
			unCrc.DDWord.hi = XmodemWaitChar(); //接收2个字节的CRC效验字
			unCrc.DDWord.lo = XmodemWaitChar();
			WatchdogClear();

			if( CalcCrc16(g_au8RecDataBuf, u32RecDataNum) == unCrc.TWord)//file://CRC校验验证
			{
				g_stRecCom.m_u16Pend = 0;

				for(u32CharCount = 0; u32CharCount < u32RecDataNum; u32CharCount += FLASH_PAGESIZE)//正确接收一帧数据
				{
					WatchdogClear();

					/*保存PC跳转指针*/
					if (APP1_START_ADDR == u32AppAddress)
					{
						if(TRUE != DRV_FlashMassiveErase(APP1_START_ADDR, APP1_END_ADDR - APP1_START_ADDR + 1))
						{
							DRV_Printf("Flash Erase Error!\n");
							return XMODEM_FLASH_ERO;
						}
					}
					else
					{

					}

					//收到128字节写入一页Flash中
					if ((u8XmodemReturn = DRV_FlashWrite(u32AppAddress, FLASH_PAGESIZE, &g_au8RecDataBuf[g_stRecCom.m_u16Pend])) == TRUE)
					{
						u32AppAddress  += FLASH_PAGESIZE; //file://Flash页加1
						g_stRecCom.m_u16Pend += FLASH_PAGESIZE;
					}
					else
					{
						CancelXModem();
						DRV_Printf("\nwrite flash fail, return = %x",u8XmodemReturn);
						DRV_Printf("0x%xH Waiting reset!\r\n",u32AppAddress);
						return  XMODEM_FLASH_ERO;
					}
				}

				g_stRecCom.m_u16Pend = 0;
				(void)XmodemPutChar(XMODEM_ACK); //正确收到一个数据块
				u32PackNo++; //数据块编号加1
				
			}
			else
			{
				(void)XmodemPutChar(XMODEM_NAK); //file://要求重发数据块
			}
			
		}
		else
		{
			if( XMODEM_SEQ_ERO == u32DelayTime)
			{
				(void)XmodemPutChar(XMODEM_NAK); //file://要求重发数据块
			}
			else
			{
				(void)XmodemPutChar(XMODEM_ACK); //重复正确收到一个数据块
			}
		}
	}
	while((u8XmodemReturn = CheckSart(&u32RecDataNum)) == XMODEM_START_OK); //循环接收，直到全部发完

	if(u8XmodemReturn == XMODEM_END_EOF)
	{
		u8XmodemReturn = XMODEM_OK;
		DRV_Printf("\r\nUpdate OK!\r\n");
	}
	else
	{
		DRV_Printf("Rtn = %d\n", u8XmodemReturn);
		DRV_Printf("\r\nXMODEM_Frame_ERO!Waiting reset!\r\n");
		u8XmodemReturn = XMODEM_FRAME_ERO;
	}
	DRV_Printf("\nLast Pack ID =[%d]",u32PackNo);


	return u8XmodemReturn;
}

/*******************************************************************************
* Function Name  	: XmodemPutChar
* Description    	: Xmodem模式输出字符
* Input          	: c -- Xmodem模式下需要输出的字符
* Output         	: None
* Return         	: Xmodem模式下需要输出的字符
* Author			: 	
* Others			: None
********************************************************************************/
int XmodemPutChar(int c)
{
    if (c == '\n')
    {
        WaitForPutChar();
        HW_REG(HAL_UART_PORT + UART_O_DR) = 0x0d;
    }

    /*等待当前任务处理完毕*/
    WaitForPutChar();

    return (HW_REG(HAL_UART_PORT + UART_O_DR) = c);
}

/*******************************************************************************
* Function Name  	: XmodemGetKey
* Description    	: 等待从键盘获取ESC按键
* Input          	: None
* Output         	: None
* Return         	: RUN_APP	 -- 运行APP
					  UPDATA_APP -- 更新APP
* Author			: 	
* Others			: None
********************************************************************************/
u8 XmodemGetKey (void)
{
	u8 u8DelayTime = 0;

	DRV_Printf("\r\nWaiting");

	while(HW_REG(HAL_UART_PORT + UART_O_FR) & UART_FR_RXFE)
	{
		WatchdogClear();
		(void)XmodemPutChar('.');
		DelayMS(5);
		u8DelayTime ++;
		if(u8DelayTime == 0xfe)
		{
			return RUN_APP;
		}
	}

	u8DelayTime = HW_REG(HAL_UART_PORT + UART_O_DR);

	if (ESC_KEY == u8DelayTime )
	{
		return UPDATA_APP;
	}
	else
	{
		return RUN_APP;
	}
}
