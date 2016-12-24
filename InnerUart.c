/*********************  系统级别的相关头文件    ***************/
#include <stdio.h>
//#include <stdint.h>
#include "stdlib.h"
#include <string.h>
#include <stdarg.h>

/*********************  bsp提供的相关头文件    ***************/
#include "uart.h"
#include "udma.h"
#include "watchdog.h"

/*********************  内部声明的相关头文件    ***************/
#include "BspConfig.h"
#include "InnerUart.h"


/*********************  内部的宏定义		      ***************/
#define IOC_UARTRXD_UART1						0x400D4108  // Selects one of the 32 pins on 
															// the four 8-pin I/O-ports (port 
															// A, port B, port C, and port D) 
															// to be the UART1 RX. 
#define DMA_CTRL_TABLE_LEN						((uint16)1024)
#define DEBUG_USART_REC_BUFF_LEN				16							//调试串口接收缓存的大小	
#define DEBUG_UART_DMA_CHANNEL					UDMA_CH22_UART1RX

//SHELL功能相关的宏定义
#define MAX_CMD_LEN								128
#define MAX_CMD_HISTORY							5
#define MAX_ARGS								(MAX_CMD_LEN >> 3)			//1//1/8
#define ENTER_KEY								0x0d
#define BACK_KEY								0x08
#define BEEP_KEY								0x07
#define UP_KEY									0x41
#define DOWN_KEY								0x42
#define RIGHT_KEY								0x43
#define LEFT_KEY								0x44
#define HOME_KEY								0x48
#define END_KEY									0x4b

#define MAX_LINE_SIZE							(100)

/************************* 内部无形参命令处理结构体 *****************/
typedef struct
{
	u8  m_ucDebugUartRecBuff[DEBUG_USART_REC_BUFF_LEN];					    //串口接收缓存
	u32 m_uiDebugUartRd;												    //串口接收缓存读指针
	u32 m_uiDebugUartWr;												    //串口接收缓存写指针
} DEBUG_USART_REC_INFOR_t;

/*********************       内部的变量	      ***************/
//static uint16 s_u16MoubleIdMax = 0;
static uint8 s_au8DmaControlTable[DMA_CTRL_TABLE_LEN] = {0};
static const char * const s_pcHex = "0123456789abcdef"; 
DEBUG_USART_REC_INFOR_t g_struDebugUartRecBuff		=		{0};		//调试串口接收缓存	
static const char StartString[]		= "*************************************************************";
static const char *Prompt			= "SHELL:\\";
char			  *SecPrompt		= "\0";
static const char *ThirPrompt		= "\\>";
static const CMD_MENU *pCMD			= NULL; //用于指向上下级菜单
static const CMD_MENU *pLastCMD		= NULL;
static const CMD_MENU *pCmdTable	= NULL;

static char			  *command		= NULL;
static char H_command[MAX_CMD_HISTORY][MAX_CMD_LEN];
static u16 EditPos					= 0; //保存当前
static u16 InsPos					= 0;
static u16 HistoryCmdPos			= 0;
static u16 CurrentCmdPos			= 0;
static u32 s_ulSetParaFlag			= 0;

static u16 s_u16MoubleIdMax			= 0;
u8		   *s_PrintfEnable			= NULL;
static char	s_ucPrintBuf[MAX_LINE_SIZE];

/*********************  内部函数	      ***************/
static int UartWrite(const char *pcBuf, uint32 ui32Len);
//static void DRV_Vprintf(const char *pcString, va_list vaArgP);
static void (*SetParaCallBack)(char *);
static void CommandHook(int argc, char * argv []);
static void RunCommand( const char* cmdline);
static int  DRV_ShellGetKeyFromDma(void);
static int GetCmdMatche( const char* cmdline );
static void EnterKeyProc(void);
static void BackKeyProc(void);
static void RightKeyProc(void);
static void LeftKeyProc(void);
static void HomeKeyProc(void);
static void EndKeyProc(void);
static void UPKeyProc(void);
static void DownKeyProc(void);
static void NormalKeyProc(u32 key );

/*******************************************************************************
* Function Name  	: WritePCA9555Config
* Description    	: 配置PCA9555的GPIO: Port0和Port1的输入和输出配置
* Input          	: u8 ucNodeAddr, u8 ucPort0Config, u8 ucPort1Config
* Output         	: None
* Return         	: None
*******************************************************************************/
void DRV_UartInit(void)
{
    // Set IO clock to the same as system clock
    SysCtrlIOClockSet(SYS_CTRL_SYSDIV_32MHZ);

    // Enable UART peripheral module
    SysCtrlPeripheralEnable(HAL_CTRL_PERIPH_UART);

    // Disable UART function
    UARTDisable(HAL_UART_PORT);

    // Disable all UART module interrupts
    UARTIntDisable(HAL_UART_PORT, 0x1FFF);

    // Set IO clock as UART clock source
    UARTClockSourceSet(HAL_UART_PORT, UART_CLOCK_PIOSC);

    // Map UART signals to the correct GPIO pins and configure them as
    // hardware controlled.
#ifdef DEBUG_UART0
    IOCPinConfigPeriphOutput(GPIO_A_BASE, GPIO_PIN_1, IOC_MUX_OUT_SEL_UART0_TXD);
    GPIOPinTypeUARTOutput(GPIO_A_BASE, GPIO_PIN_1); 
    IOCPinConfigPeriphInput(GPIO_A_BASE, GPIO_PIN_0, IOC_UARTRXD_UART0);
    GPIOPinTypeUARTInput(GPIO_A_BASE, GPIO_PIN_0);
#else
	IOCPinConfigPeriphOutput(GPIO_D_BASE, GPIO_PIN_1, IOC_MUX_OUT_SEL_UART1_TXD);
	IOCPinConfigPeriphInput(GPIO_D_BASE, GPIO_PIN_0,IOC_UARTRXD_UART1);
	GPIOPinTypeUARTInput(GPIO_D_BASE, GPIO_PIN_0);
	GPIOPinTypeUARTOutput(GPIO_D_BASE, GPIO_PIN_1);
#endif
    //
    // Configure the UART for 115,200, 8-N-1 operation.
    // This function uses SysCtrlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    //
    UARTConfigSetExpClk(HAL_UART_PORT, 
                                        SysCtrlClockGet(), 
                                        115200,
                                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTEnable(HAL_UART_PORT);
}

/*******************************************************************************
* Function Name  	: WritePCA9555Config
* Description    	: 配置PCA9555的GPIO: Port0和Port1的输入和输出配置
* Input          	: u8 ucNodeAddr, u8 ucPort0Config, u8 ucPort1Config
* Output         	: None
* Return         	: None
*******************************************************************************/
void DRV_UartDmaInit(void)
{
	uDMAEnable();											// 使能DMA模块
	uDMAControlBaseSet(s_au8DmaControlTable);				// 必须对DMA的控制表进行设置

	uDMAChannelAttributeDisable(DEBUG_UART_DMA_CHANNEL, 	// DMA通道UART1 RX
								UDMA_ATTR_USEBURST | 		// 清除仅仅响应脉冲请求
								UDMA_ATTR_HIGH_PRIORITY |	// 清除通道高优先级
								UDMA_ATTR_ALTSELECT |		// 清除使用交替结构
								UDMA_ATTR_REQMASK);			// 清除外设不响应
	/*
	uDMAChannelAttributeEnable(DEBUG_UART_DMA_CHANNEL, 
								UDMA_ATTR_ALTSELECT);*/

	uDMAChannelControlSet(DEBUG_UART_DMA_CHANNEL | UDMA_PRI_SELECT, 	// 设置UART0 RX主结构寄存器
						  UDMA_SIZE_8 | UDMA_SRC_INC_NONE | 			// 数据类型8位, 源地址不累加
						  UDMA_DST_INC_8 | UDMA_ARB_8);					// 目的地址累加8，仲裁值8 
	/*
	uDMAChannelControlSet(DEBUG_UART_DMA_CHANNEL | UDMA_ALT_SELECT, 	// 设置UART0 RX主结构寄存器
						  UDMA_SIZE_8 | UDMA_SRC_INC_NONE | 			// 数据类型8位, 源地址不累加
						  UDMA_DST_INC_8 | UDMA_ARB_8);					// 目的地址累加8，仲裁值8 
*/

	uDMAChannelTransferSet(DEBUG_UART_DMA_CHANNEL | UDMA_PRI_SELECT, 	// 设置UART0 RX主结构寄存器
						   UDMA_MODE_BASIC, 							// 基本模式
						  (void *)HAL_UART_PORT, 						// 源地址 0x4000D000
						  (void *)&(g_struDebugUartRecBuff.m_ucDebugUartRecBuff[0]),	// 目的地址 
						  DEBUG_USART_REC_BUFF_LEN);	
	/*// 传输量为8
	uDMAChannelTransferSet(DEBUG_UART_DMA_CHANNEL | UDMA_ALT_SELECT, 	// 设置UART0 RX主结构寄存器
						  UDMA_MODE_PINGPONG, 							// 基本模式
						  (void *)HAL_UART_PORT, 						// 源地址 0x4000D000
						  (void *)&(g_struDebugUartRecBuff.m_ucDebugUartRecBuff[DEBUG_USART_REC_BUFF_LEN/2]), 	// 目的地址 
						  DEBUG_USART_REC_BUFF_LEN/2);	
*/
	uDMAChannelEnable(DEBUG_UART_DMA_CHANNEL);							// 使能UART0 RX通道
	UARTDMAEnable(HAL_UART_PORT, UART_DMA_RX);							// 使能UART RX的DMA
}

/**************************************************************************************************
Function: 	DRV_PrintfRegister
Description:注册打印
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
void DRV_PrintfRegister(u16 u16MoudleIdMax)
{
	u16 u16MoudleIdStorageLen = 0;
	s_u16MoubleIdMax = u16MoudleIdMax;

	u16MoudleIdStorageLen =  (s_u16MoubleIdMax / 8) + (((s_u16MoubleIdMax % 8) > 0) ? 1 : 0);

	if (NULL == s_PrintfEnable)
	{
		s_PrintfEnable = (u8*)malloc(u16MoudleIdStorageLen);
	}

	if (NULL != s_PrintfEnable)
	{
		memset(s_PrintfEnable, 0, u16MoudleIdStorageLen);
	}
}

/**************************************************************************************************
Function: 	DRV_PrintfRegister
Description:注册打印
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
void DRV_SetMoudlePrintfEnable(u16 u16MoudleId, u8 u8EnableStatus)
{
	if (u16MoudleId >= s_u16MoubleIdMax)
	{
		return;
	}

	if (u8EnableStatus)
	{
		s_PrintfEnable[u16MoudleId / 8] |= (0x01 << (u16MoudleId%8));
	}
	else
	{
		s_PrintfEnable[u16MoudleId / 8] &= ~(0x01 << (u16MoudleId%8));
	}
}

/*******************************************************************************
* Function Name  	: DRV_Vprintf
* Description    	: 配置PCA9555的GPIO: Port0和Port1的输入和输出配置
* Input          	: pcString, vaArgP
* Output         	: None
* Return         	: None
*******************************************************************************/
static int UartWrite(const char *pcBuf, uint32 ui32Len) 
{
	uint32 u32Idx = 0;
	
	//
	// Check for valid UART base address, and valid arguments.
	//
	if ((NULL == pcBuf) ||
		(0 == ui32Len))
	{
		return 0;
	}

	for(u32Idx = 0; u32Idx < ui32Len; u32Idx++) 
	{
		if(pcBuf[u32Idx] == '\n') 
		{
			UARTCharPut(HAL_UART_PORT, '\r'); 
		}
		UARTCharPut(HAL_UART_PORT, pcBuf[u32Idx]); 
	}

	return(u32Idx);
}

/*******************************************************************************
* Function Name  	: CancelXModem
* Description    	: 取消XMODEM模式
* Input          	: None
* Output         	: None
* Return         	: None
*******************************************************************************/
static void WaitForPutChar(void)
{
	uint8 u8Rec=0;
	uint16 u16DeyTime = 0;

	//while(HWREG(HAL_UART_PORT + UART_O_FR) & UART_FR_BUSY)
	while(HWREG(HAL_UART_PORT + UART_O_FR) & UART_FR_TXFF)
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
* Function Name  	: XmodemPutChar
* Description    	: interrupt controlled XmodemGetKey
* Input          	: None
* Output         	: None
* Return         	: u8Rec
*******************************************************************************/
u32  DRV_ShellSendChar(u32 ch)
{
	if (ch == '\n')
	{
		WaitForPutChar();
		HWREG(HAL_UART_PORT + UART_O_DR) = 0x0d;
	}

	/*等待当前任务处理完毕*/
	WaitForPutChar();

	return (HWREG(HAL_UART_PORT + UART_O_DR) = ch);
}

/**************************************************************************************************
Function: 	DRV_ShellGetKeyFromDma
Description:调试串口发送字符
Input: 		None
Output: 	None
Returns: 	int					-- 获取到的字符
Author: 	
Others: 	None
***************************************************************************************************/
int  DRV_ShellGetKeyFromDma(void)
{
	u8 RemainCount;

	RemainCount = uDMAChannelSizeGet(DEBUG_UART_DMA_CHANNEL);
	/*DRV_Printf("Re:%d-%d-%d-%d\n", RemainCount, 
								   g_struDebugUartRecBuff.m_uiDebugUartWr,
								   g_struDebugUartRecBuff.m_uiDebugUartRd,
								   sizeof(g_struDebugUartRecBuff.m_ucDebugUartRecBuff));*/

	g_struDebugUartRecBuff.m_uiDebugUartWr = (sizeof(g_struDebugUartRecBuff.m_ucDebugUartRecBuff) - RemainCount)
		% (sizeof(g_struDebugUartRecBuff.m_ucDebugUartRecBuff));
	g_struDebugUartRecBuff.m_uiDebugUartRd = g_struDebugUartRecBuff.m_uiDebugUartRd % (sizeof(g_struDebugUartRecBuff.m_ucDebugUartRecBuff));
	if (g_struDebugUartRecBuff.m_uiDebugUartWr != g_struDebugUartRecBuff.m_uiDebugUartRd)
	{
		//DRV_Printf("-%c-!", g_struDebugUartRecBuff.m_ucDebugUartRecBuff[g_struDebugUartRecBuff.m_uiDebugUartRd]);
		if (0 == RemainCount)
		{
			DRV_UartDmaInit();
			//uDMAChannelRequest(DEBUG_UART_DMA_CHANNEL);
		}

		return g_struDebugUartRecBuff.m_ucDebugUartRecBuff[g_struDebugUartRecBuff.m_uiDebugUartRd++];
	}

	return -1;
}
/*
int  DRV_ShellGetKeyFromDma(void)
{
	u8 RemainCount;

	RemainCount = uDMAChannelSizeGet(UDMA_CH22_UART1RX);
	//DRV_Printf("RemainCount:%d\n", RemainCount);
	g_struDebugUartRecBuff.m_uiDebugUartWr = (sizeof(g_struDebugUartRecBuff.m_ucDebugUartRecBuff) - RemainCount)
		% (sizeof(g_struDebugUartRecBuff.m_ucDebugUartRecBuff));
	g_struDebugUartRecBuff.m_uiDebugUartRd = g_struDebugUartRecBuff.m_uiDebugUartRd % (sizeof(g_struDebugUartRecBuff.m_ucDebugUartRecBuff));
	if (g_struDebugUartRecBuff.m_uiDebugUartWr != g_struDebugUartRecBuff.m_uiDebugUartRd)
	{
		//DRV_Printf("-%c%!", g_struDebugUartRecBuff.m_ucDebugUartRecBuff[g_struDebugUartRecBuff.m_uiDebugUartRd]);
		return g_struDebugUartRecBuff.m_ucDebugUartRecBuff[g_struDebugUartRecBuff.m_uiDebugUartRd++];
	}

	return -1;
}*/
/*******************************************************************************
* Function Name  	: XmodemWaitChar
* Description    	: //file://等待从RS232接收一个有效的字节
* Input          	: None
* Output         	: None
* Return         	: u8Rec
*******************************************************************************/
static uint8 DRV_WaitChar(void)
{
	uint8 u8Rec=0;
	uint16 u16DeyTime = 0;

	while(HWREG(HAL_UART_PORT + UART_O_FR) & UART_FR_RXFE)
	{
		WatchdogClear();
		u16DeyTime++;
		if(0xffff == u16DeyTime)
		{
			u8Rec++;
			if(u8Rec > 40)
			{
				return 0xff; /*ero*/
			}
		}
	}
	u8Rec = HWREG(HAL_UART_PORT + UART_O_DR);
	//DRV_Printf("%x ", u8Rec);

	return u8Rec;
}

/**************************************************************************************************
Function: 	DRV_ShellGetChar
Description:从串口获取一个字符
Input: 		None
Output: 	None
Returns: 	static void
Author: 	
Others: 	None
***************************************************************************************************/
int DRV_ShellGetChar(void)
{
	int ch;
	int loop = 3000;

	ch = -1;
	while (-1 == ch)
	{
		ch = DRV_ShellGetKeyFromDma();//DRV_WaitChar();//
		if (loop-- < 1)
		{
			break;
		}
	}

	return ch;
}

/**************************************************************************************************
Function: 	DRV_ShellInit
Description:Shell后台初始化
Input: 		pstCmdMenu			 -- 顶级菜单的起始地址
Output: 	None
Returns: 	0-成功 其他--失败
Author: 	
Others: 	None
***************************************************************************************************/
u32 DRV_ShellInit(const CMD_MENU* pstCmdMenu)
{
	int i;

	for (i = 0; i < MAX_CMD_HISTORY; i++)
	{
		H_command[i][0] = '\0';
	}

	command = H_command[0];
	EditPos = 0;
	InsPos = 0;
	HistoryCmdPos = 0;
	CurrentCmdPos = 0;

	pCMD = pstCmdMenu;
	pCmdTable = pCMD;
	pLastCMD = pCMD;
	s_ulSetParaFlag = FALSE;
	SetParaCallBack = NULL;
	DRV_Printf("%s\r\n", StartString);
	DRV_Printf( "%s%s%s", Prompt, SecPrompt, ThirPrompt);

	return 0;
}

/**************************************************************************************************
Function: 	DRV_ShellPrintPrompt
Description:打印提示信息
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
void DRV_ShellPrintPrompt(void)
{
	DRV_Printf( "%s%s%s", Prompt, SecPrompt, ThirPrompt);
}

/**************************************************************************************************
Function: 	ChangeShellMode
Description:Shell命令模式和参数设置模式切换函数
Input: 		void *pt  ,char *pPrompt
Output: 	None
Returns: 	static void
Author: 	
Others: 	None
***************************************************************************************************/
static void ChangeShellMode(char *pPrompt, setPara sp)
{
	if (sp == NULL)
	{
		s_ulSetParaFlag = FALSE;
	}
	else
	{
		s_ulSetParaFlag = TRUE;
		SetParaCallBack = sp;
		DRV_Printf("%s", pPrompt);
	}
}

/**************************************************************************************************
Function: 	ParseArgs
Description:命令参数预处理
Input: 		char* cmdline ,  char** argv
Output: 	None
Returns: 	static void
Author: 	
Others: 	None
***************************************************************************************************/
static int ParseArgs( char* cmdline, char** argv )
{
#define STATE_WHITESPACE 0
#define STATE_WORD 1

	char* c;
	int state = STATE_WHITESPACE;
	int i;

	i = 0;

	if (strlen( cmdline ) == 0)
	{
		return i;
	}

	/* convert all tabs into single spaces */
	c = cmdline;
	while (*c != '\0')
	{
		if (*c == '\t')
		{
			*c = ' ';
		}

		c++;
	}

	c = cmdline;

	/* now find all words on the command line */
	while (*c != '\0')
	{
		if (state == STATE_WHITESPACE)
		{
			if ((*c != ' ') && (i < MAX_ARGS))
			{
				argv[i] = c;        //make argv[i] point to c
				i++;
				state = STATE_WORD;
			}
		}
		else
		{
			/* state == STATE_WORD */
			if (*c == ' ')
			{
				*c = '\0';
				state = STATE_WHITESPACE;
			}
		}

		c++;
	}

	return i;
#undef STATE_WHITESPACE
#undef STATE_WORD
}

/**************************************************************************************************
Function: 	GetCmdMatche
Description:查找匹配命令
Input: 		char* cmdline
Output: 	None
Returns: 	-1:无匹配命令;不等于-1:命令数组的偏移值
Author: 	
Others: 	None
***************************************************************************************************/
int GetCmdMatche( const char* cmdline )
{
	int i;

	for (i = 0; pCMD[i].cmd != NULL; i++)
	{
		if (strlen( cmdline ) != strlen( pCMD[i].cmd ))
		{
			continue;
		}

		if (strncmp( pCMD[i].cmd, cmdline, strlen( pCMD[i].cmd )) == 0)
		{
			return i;
		}
	}

	return -1;
}

/**************************************************************************************************
Function: 	RunCommand
Description:后台命令处理，调用命令处理函数
Input: 		char* cmdline
Output: 	None
Returns: 	0:有相应处理函数;－1:没有查到匹配命令处理函数
Author: 	
Others: 	None
***************************************************************************************************/
static void RunCommand( const char* cmdline)
{
	int i;
	int argc, num_commands;
	char * argv[MAX_ARGS];
	static char temp_cmd_line[MAX_CMD_LEN] = {0};

	i = 0;
	while (cmdline[i] != '\0')
	{
		temp_cmd_line[i] = cmdline[i];
		i++;
	}

	temp_cmd_line[i] = '\0';
	argc = ParseArgs( temp_cmd_line, argv );

	/* only whitespace */
	if (argc == 0)
	{
		return;
	}

	num_commands = GetCmdMatche( argv[0] );
	if (num_commands < 0)
	{
		CommandHook( argc, argv );
	}
	else if (pCMD[num_commands].proc != NULL)
	{
		pCMD[num_commands].proc( argc, argv );
	}

	return;
}

/*******************************************************************************
* Function Name  	: DRV_Vprintf
* Description    	: 配置PCA9555的GPIO: Port0和Port1的输入和输出配置
* Input          	: pcString, vaArgP
* Output         	: None
* Return         	: None
*******************************************************************************/
/*
static void DRV_Vprintf(const char *pcString, va_list vaArgP) 
{
	uint32 ui32Idx, ui32Value, ui32Pos, ui32Count, ui32Base, ui32Neg; 
	char *pcStr, pcBuf[16], cFill; 

	if (NULL == pcString)
	{
		return;
	}

	while(*pcString) 
	{
		//1，将不需要转义的字符直接打出来
		for(ui32Idx = 0; (pcString[ui32Idx] != '%') && (pcString[ui32Idx] != '\0'); ui32Idx++) 
		{
		}
		UartWrite(pcString, ui32Idx); 
		pcString += ui32Idx; 

		if(*pcString == '%') 
		{
			pcString++; 

			ui32Count = 0; 
			cFill = ' '; 
again:
			switch(*pcString++) 
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				{
					if((pcString[-1] == '0') && (ui32Count == 0)) 
					{
						cFill = '0'; 
					}

					//
					// Update the digit count.
					//
					ui32Count *= 10; 
					ui32Count += pcString[-1] - '0'; 

					//
					// Get the next character.
					//
					goto again;
				}

				//
				// Handle the %c command.
				//
			case 'c':
				{
					//
					// Get the value from the varargs.
					//
					ui32Value = va_arg(vaArgP, uint32_t); 

					//
					// Print out the character.
					//
					UartWrite((char *)&ui32Value, 1); 

					//
					// This command has been handled.
					//
					break;
				}

				//
				// Handle the %d and %i commands.
				//
			case 'd':
			case 'i':
				{
					//
					// Get the value from the varargs.
					//
					ui32Value = va_arg(vaArgP, uint32_t); 

					//
					// Reset the buffer position.
					//
					ui32Pos = 0; 

					//
					// If the value is negative, make it positive and indicate
					// that a minus sign is needed.
					//
					if((int32)ui32Value < 0) 
					{
						//
						// Make the value positive.
						//
						ui32Value = -(int32)ui32Value; 

						//
						// Indicate that the value is negative.
						//
						ui32Neg = 1; 
					}
					else
					{
						//
						// Indicate that the value is positive so that a minus
						// sign isn't inserted.
						//
						ui32Neg = 0; 
					}

					//
					// Set the base to 10.
					//
					ui32Base = 10; 

					//
					// Convert the value to ASCII.
					//
					goto convert;
				}

				//
				// Handle the %s command.
				//
			case 's':
				{
					//
					// Get the string pointer from the varargs.
					//
					pcStr = va_arg(vaArgP, char *); 

					//
					// Determine the length of the string.
					//
					for(ui32Idx = 0; pcStr[ui32Idx] != '\0'; ui32Idx++) 
					{
					}

					//
					// Write the string.
					//
					UartWrite(pcStr, ui32Idx); 

					//
					// Write any required padding spaces
					//
					if(ui32Count > ui32Idx) 
					{
						ui32Count -= ui32Idx; 
						while(ui32Count--) 
						{
							UartWrite(" ", 1);
						}
					}

					//
					// This command has been handled.
					//
					break;
				}

				//
				// Handle the %u command.
				//
			case 'u':
				{
					//
					// Get the value from the varargs.
					//
					ui32Value = va_arg(vaArgP, uint32_t); 

					//
					// Reset the buffer position.
					//
					ui32Pos = 0; 

					//
					// Set the base to 10.
					//
					ui32Base = 10; 

					//
					// Indicate that the value is positive so that a minus sign
					// isn't inserted.
					//
					ui32Neg = 0; 

					//
					// Convert the value to ASCII.
					//
					goto convert;
				}

				//
				// Handle the %x and %X commands.  Note that they are treated
				// identically; i.e. %X will use lower case letters for a-f
				// instead of the upper case letters is should use.  We also
				// alias %p to %x.
				//
			case 'x':
			case 'X':
			case 'p':
				{
					//
					// Get the value from the varargs.
					//
					ui32Value = va_arg(vaArgP, uint32_t); 

					//
					// Reset the buffer position.
					//
					ui32Pos = 0; 

					//
					// Set the base to 16.
					//
					ui32Base = 16; 

					//
					// Indicate that the value is positive so that a minus sign
					// isn't inserted.
					//
					ui32Neg = 0; 

					//
					// Determine the number of digits in the string version of
					// the value.
					//
convert:
					for(ui32Idx = 1; 
						(((ui32Idx * ui32Base) <= ui32Value) && 
						(((ui32Idx * ui32Base) / ui32Base) == ui32Idx)); 
					ui32Idx *= ui32Base, ui32Count--) 
					{
					}

					//
					// If the value is negative, reduce the count of padding
					// characters needed.
					//
					if(ui32Neg) 
					{
						ui32Count--; 
					}

					//
					// If the value is negative and the value is padded with
					// zeros, then place the minus sign before the padding.
					//
					if(ui32Neg && (cFill == '0')) 
					{
						//
						// Place the minus sign in the output buffer.
						//
						pcBuf[ui32Pos++] = '-'; 

						//
						// The minus sign has been placed, so turn off the
						// negative flag.
						//
						ui32Neg = 0; 
					}

					//
					// Provide additional padding at the beginning of the
					// string conversion if needed.
					//
					if((ui32Count > 1) && (ui32Count < 16)) 
					{
						for(ui32Count--; ui32Count; ui32Count--) 
						{
							pcBuf[ui32Pos++] = cFill; 
						}
					}

					//
					// If the value is negative, then place the minus sign
					// before the number.
					//
					if(ui32Neg) 
					{
						//
						// Place the minus sign in the output buffer.
						//
						pcBuf[ui32Pos++] = '-'; 
					}

					//
					// Convert the value into a string.
					//
					for(; ui32Idx; ui32Idx /= ui32Base) 
					{
						pcBuf[ui32Pos++] = s_pcHex[(ui32Value / ui32Idx) % ui32Base]; 
					}

					//
					// Write the string.
					//
					UartWrite(pcBuf, ui32Pos); 

					//
					// This command has been handled.
					//
					break;
				}

				//
				// Handle the %% command.
				//
			case '%':
				{
					//
					// Simply write a single %.
					//
					UartWrite(pcString - 1, 1); 

					//
					// This command has been handled.
					//
					break;
				}

				//
				// Handle all other commands.
				//
			default:
				{
					//
					// Indicate an error.
					//
					UartWrite("ERROR", 5);

					//
					// This command has been handled.
					//
					break;
				}
			}
		}
	}
}
*/
/**************************************************************************************************
Function: 	CommandHook
Description:打印信息，提示找不到命令
Input: 		int argc , char * argv[]
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void CommandHook(int argc, char * argv []  )
{
	DRV_Printf( "No '%s' command, please type 'help' or '?' for a command list.\n",
		argv[0] );
}


/**************************************************************************************************
Function: 	EnterKeyProc
Description:回车键处理
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void EnterKeyProc(void)
{
	if (FALSE == s_ulSetParaFlag) //命令模式
	{
		DRV_Printf( "\n" );
		if (EditPos)
		{
			RunCommand( command);
			CurrentCmdPos = (CurrentCmdPos + 1) % MAX_CMD_HISTORY;
			HistoryCmdPos = CurrentCmdPos;
			command = H_command[CurrentCmdPos];
		}
	}
	else                        //参数设置模式
	{
		if (EditPos == 0)
		{
			return;
		}

		if (SetParaCallBack != NULL)
		{
			SetParaCallBack(command);
		}
	}

	if (FALSE == s_ulSetParaFlag)
	{
		DRV_Printf( "%s%s%s", Prompt, SecPrompt, ThirPrompt);
	}

	EditPos = 0;
	InsPos = 0;
}

/**************************************************************************************************
Function: 	BackKeyProc
Description:退格键处理
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void BackKeyProc(void)
{
	u32 x;

	if (0 == InsPos)
	{
		(void)putch(BEEP_KEY);
		return;
	}

	if (EditPos == InsPos)
	{
		EditPos--;
		InsPos--;
		DRV_Printf( "\b \b" );
	}
	else
	{
		EditPos--;
		InsPos--;
		for (x = InsPos; x < EditPos; x++)
		{
			command[x] = command[x + 1];
		}

		command[x] = ' ';
		DRV_Printf( "\b \b" );
		for (x = InsPos; x < EditPos + 1; x++)
		{
			(void)putch(command[x]);
		}

		for (x = 0; x < (u32)(EditPos + 1 - InsPos); x++)
		{
			(void)putch( 0x1b );
			(void)putch( 0x5b );
			(void)putch( LEFT_KEY );
		}
	}

	command[EditPos] = '\0';
}

/**************************************************************************************************
Function: 	RightKeyProc
Description:向右方向键处理
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void RightKeyProc(void)
{
	if (InsPos == EditPos)
	{
		(void)putch( BEEP_KEY );
		return;
	}

	(void)putch( 0x1b );
	(void)putch( 0x5b );
	(void)putch( RIGHT_KEY );
	InsPos++;
}

/**************************************************************************************************
Function: 	LeftKeyProc
Description:向左方向键处理
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void LeftKeyProc(void)
{
	if (InsPos == 0)
	{
		(void)putch( 0x1b );
		(void)putch( 0x5b );
		(void)putch( BEEP_KEY );
		return;
	}

	(void)putch( 0x1b );
	(void)putch( 0x5b );
	(void)putch( LEFT_KEY );
	InsPos--;
}

/**************************************************************************************************
Function: 	HomeKeyProc
Description:HOME键处理
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void HomeKeyProc(void)
{
	u32 x;

	for (x = 0; x < InsPos; x++)
	{
		(void)putch( 0x1b );
		(void)putch( 0x5b );
		(void)putch( LEFT_KEY );
	}

	InsPos = 0;
}

/**************************************************************************************************
Function: 	EndKeyProc
Description:End键处理
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void EndKeyProc(void)
{
	u32 x;

	for (x = InsPos; x < EditPos; x++)
	{
		(void)putch( 0x1b );
		(void)putch( 0x5b );
		(void)putch( RIGHT_KEY );
	}

	InsPos = EditPos;
}

/**************************************************************************************************
Function: 	UPKeyProc
Description:UP键处理
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void UPKeyProc(void)
{
	u32 x;
	u32 cmd_len;
	u16 save_pos;

	save_pos = HistoryCmdPos;
	if (0 == HistoryCmdPos)
	{
		HistoryCmdPos = MAX_CMD_HISTORY - 1;
	}
	else
	{
		HistoryCmdPos--;
	}

	if (HistoryCmdPos == CurrentCmdPos)
	{
		HistoryCmdPos = save_pos;
		(void)putch(BEEP_KEY);
		return;
	}

	cmd_len = strlen( H_command[HistoryCmdPos] );

	memcpy(command, (char *)H_command[HistoryCmdPos], cmd_len + 1);

	for (x = 0; x < InsPos; x++)
	{
		//回到行首
		(void)putch( 0x1b );
		(void)putch( 0x5b );
		(void)putch( LEFT_KEY );
	}

	for (x = 0; x < cmd_len; x++)
	{
		(void)putch( command[x] );
	}

	for (; x < EditPos; x++)
	{
		(void)putch( ' ' );
	}

	for (; x > cmd_len; x--)
	{
		(void)putch( 0x1b );
		(void)putch( 0x5b );
		(void)putch( LEFT_KEY );
	}

	EditPos = InsPos = (u16)cmd_len;
}
//lint -restore
/**************************************************************************************************
Function: 	DownKeyProc
Description:Down键处理
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void DownKeyProc(void)
{
	u32 x;
	u32 cmd_len;

	if (HistoryCmdPos == CurrentCmdPos)
	{
		(void)putch(BEEP_KEY);
		return;
	}

	HistoryCmdPos++;
	HistoryCmdPos %= MAX_CMD_HISTORY;

	cmd_len = strlen( H_command[HistoryCmdPos] );
	memcpy(command, (char *)H_command[HistoryCmdPos], cmd_len + 1);

	for (x = 0; x < InsPos; x++)
	{
		//回到行首
		(void)putch( 0x1b );
		(void)putch( 0x5b );
		(void)putch( LEFT_KEY );
	}

	for (x = 0; x < cmd_len; x++)
	{
		(void)putch( command[x] );
	}

	for (; x < EditPos; x++)
	{
		(void)putch( ' ' );
	}

	for (; x > cmd_len; x--)
	{
		(void)putch( 0x1b );
		(void)putch( 0x5b );
		(void)putch( LEFT_KEY );
	}

	EditPos = InsPos = (u16)cmd_len;
}

/**************************************************************************************************
Function: 	NormalKeyProc
Description:可见字符按键处理
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void NormalKeyProc(u32 key )
{
	u32 x;

	if ((EditPos == 0) && (InsPos == 0) && (key == '?'))
	{
		RunCommand("help");
		DRV_Printf( "%s%s%s", Prompt, SecPrompt, ThirPrompt);
		return;
	}

	if (EditPos == InsPos)
	{
		command[EditPos++] = (char)key;
		(void)putch( key );
		InsPos++;
	}
	else
	{
		for (x = EditPos; x > InsPos; x--)
		{
			command[x] = command[x - 1];
		}

		command[InsPos] = (char)key;
		EditPos++;
		for (x = InsPos; x < EditPos; x++)
		{
			(void)putch( command[x] );
		}

		InsPos++;
		for (x = 0; x < (u32)(EditPos - InsPos); x++)
		{
			(void)putch( 0x1b );
			(void)putch( 0x5b );
			(void)putch( LEFT_KEY );
		}
	}

	command[EditPos] = '\0';
}


/**************************************************************************************************
Function: 	DRV_ShellEnterMenu
Description:进入特定命令菜单
Input: 		CMD_MENU *pCmd_menu
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
void DRV_ShellEnterMenu(const CMD_MENU *pCmd_menu, const char *pSecPromt)
{
	s32 index;

	index = GetCmdMatche(pSecPromt);
	SecPrompt = pCmdTable[index].cmd;
	pLastCMD = pCMD;
	pCMD = pCmd_menu;
	DRV_ShellHelp(0, 0);
}

/**************************************************************************************************
Function: 	ExitCurrentMenu
Description:退出当前命令菜单,返回上一级菜单
Input: 		None
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
static void ExitCurrentMenu(void)
{
	pCMD = pLastCMD;
	DRV_ShellHelp(0, 0);
	SecPrompt = "\0";
}

/**************************************************************************************************
Function: 	DRV_ShellHelp
Description:打印后台帮助信息
Input: 		int argc , char * argv[]
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
void DRV_ShellHelp( int argc, char * argv[] )
{
	int i;

	DRV_Printf("\n");
	for (i = 0; pCMD[i].cmd != NULL; i++)
	{
		if (pCMD[i].hlp != NULL)
		{
			//            printf( "%-12s  : %s\n", pCMD[i].cmd, pCMD[i].hlp );
			DRV_Printf( "%s  : %s\r\r\n", pCMD[i].cmd, pCMD[i].hlp );
		}
	}

	return;
}

/**************************************************************************************************
Function: 	DRV_ShellExit
Description:退出当前菜单
Input: 		int argc , char * argv[]
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
void DRV_ShellExit(int argc, char * argv [])
{
	ExitCurrentMenu();
}

/**************************************************************************************************
Function: 	JudgeNormalKeyValid
Description:判断可见字符是否有效
Input: 		key
Output: 	None
Returns: 	1--有效，其他--无效
Author: 	
Others: 	None
***************************************************************************************************/
static BOOL JudgeNormalKeyValid(s32 key)
{
	return (BOOL)((key >= 0x20) && (key <= 0x7e) && (EditPos < (MAX_CMD_LEN - 1)));
}

/**************************************************************************************************
Function: 	DRV_ShellProcRecvKey
Description:字符处理函数
Input: 		key
Output: 	None
Returns: 	None
Author: 	
Others: 	None
***************************************************************************************************/
void DRV_ShellProcRecvKey(s32 key)
{
	switch (key)
	{
	case ENTER_KEY:
		EnterKeyProc();
		break;
	case BACK_KEY:
	case 0x7F:
		BackKeyProc();
		break;
	case 0x1b:
		key = DRV_ShellGetChar();
		if (key != 0x5b)
		{
			break;
		}

		key = DRV_ShellGetChar();
		if (key == RIGHT_KEY)
		{
			RightKeyProc();
		}
		else if (key == LEFT_KEY)
		{
			LeftKeyProc();
		}
		else if (key == HOME_KEY)
		{
			HomeKeyProc();
		}
		else if (key == END_KEY)
		{
			EndKeyProc();
		}
		else if (key == UP_KEY)
		{
			UPKeyProc();
		}
		else if (key == DOWN_KEY)
		{
			DownKeyProc();
		}

		break;
	default:

		if (JudgeNormalKeyValid(key) == TRUE)
		{
			NormalKeyProc((u32)key);
		}

		break;
	}
}

/*******************************************************************************
* Function Name  	: WritePCA9555Config
* Description    	: 配置PCA9555的GPIO: Port0和Port1的输入和输出配置
* Input          	: u8 ucNodeAddr, u8 ucPort0Config, u8 ucPort1Config
* Output         	: None
* Return         	: None
*******************************************************************************/
void DRV_Printf(const char *pFmt, ...)//u16 u16MoudleId, 
{
	u16 i = 0;
/*
	if (u16MoudleId >= s_u16MoubleIdMax)
	{
		return;
	}

	if (s_PrintfEnable[u16MoudleId / 8] & (0x01 << (u16MoudleId%8)))
	{*/
		va_list argptr;	

		va_start(argptr, pFmt);			
		vsnprintf(s_ucPrintBuf, MAX_LINE_SIZE, pFmt, argptr);
		s_ucPrintBuf[MAX_LINE_SIZE-1] = 0;     
		va_end(argptr);

		for (i = 0; i < strlen(s_ucPrintBuf); i++)
		{
			DRV_ShellSendChar(s_ucPrintBuf[i]);
			//printf("%c", s_ucPrintBuf[i]);
		}
	//}
}
