#include "ir.h"

volatile uint8_t pulse_bnum=0;		 // 脉冲计数器
volatile uint16_t ir_buf[68];		 // 装载用户码和数据码
uint8_t custom_code_hi;
uint8_t custom_code_lo;
uint8_t data_code_hi;
uint8_t data_code_lo;

void IE_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;// 定义一个GPIO_InitTypeDef类型的变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// 允许GPIOB时钟
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1; // 选择PIN1
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; // 通用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz速度
	GPIO_Init(GPIOB, &GPIO_InitStruct);// 把参数带进函数配置
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉低
}

volatile void delay8_77us()
{
	uint8_t i=65;
	while(i--);
}

volatile void delay17_53us()
{
	uint8_t i=130;
	while(i--);
}

void IE_Send_zero()
{
	uint8_t i;
	// 560us高电平
	for(i=0;i<21;i++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉高
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉低
		delay17_53us();
	}
	//  560us低电平
	for(i=0;i<21;i++)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉高
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉低
		delay17_53us();
	}
}

void IE_Send_one()
{
	uint8_t i;
	// 560us高电平
	for(i=0;i<21;i++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉高
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉低
		delay17_53us();
	}
	//  1680us低电平
	for(i=0;i<64;i++)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉高
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉低
		delay17_53us();
	}
}

void IE_Send_Data(uint8_t value)
{
	uint8_t i;
	
	for(i=0;i<8;i++)
	{
		if((value&0x80)==0x80)
			IE_Send_one();
		else
			IE_Send_zero();
		value<<=1;
	}
}

void IE_T(uint8_t custom_code_hi, uint8_t custom_code_lo, uint8_t data_code)
{
	uint16_t i;
	
	// 发送9ms高电平
	for(i=0;i<342;i++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉高
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉低
		delay17_53us();
	}
	// 发送4.5ms低电平
	for(i=0;i<171;i++)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉高
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//把GPIOB1引脚拉低
		delay17_53us();
	}
	// 发送定制码高字节
	IE_Send_Data(custom_code_hi);
	// 发送定制码低字节
	IE_Send_Data(custom_code_lo);
	// 发送数据码
	IE_Send_Data(data_code);
	// 发送数据码反码
	IE_Send_Data(~data_code);
	// 发送停止位
	IE_Send_zero();
}

void IR_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStruct; 
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	/* 配置引脚 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);												
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	// 上拉输入模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStruct);	

	/* 配置引脚中断模式 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0); // PB0
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //双边沿中断
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
  EXTI_Init(&EXTI_InitStruct);
	
	 /* 配置NVIC */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;// 响应优先级
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; 
  NVIC_Init(&NVIC_InitStruct);
	
	/* 设置定时器6每20ms中断 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);//允许定时器6
	TIM_TimeBaseInitStruct.TIM_Prescaler= 71;
	TIM_TimeBaseInitStruct.TIM_Period=20000;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	TIM_ClearFlag(TIM6, TIM_IT_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;	  
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;	// 响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/* 打开定时器6 */
	TIM_Cmd(TIM6, ENABLE);
}

uint8_t ir_process(void)
{
	uint8_t i;
	uint16_t buf;

	for(i=3;i<19;i++)
	{
		custom_code_hi<<=1;
		buf=ir_buf[i]+ir_buf[++i];
		if((buf>2100)&&(buf<2450))
		{
			custom_code_hi+=1;
		}
	}
	for(i=19;i<35;i++)
	{
		custom_code_lo<<=1;
		buf=ir_buf[i]+ir_buf[++i];
		if((buf>2100)&&(buf<2450))
		{
			custom_code_lo+=1;
		}
	}
	for(i=35;i<51;i++)
	{
		data_code_hi<<=1;
		buf=ir_buf[i]+ir_buf[++i];
		if((buf>2100)&&(buf<2450))
		{
			data_code_hi+=1;
		}
	}
	for(i=51;i<67;i++)
	{
		data_code_lo<<=1;
		buf=ir_buf[i]+ir_buf[++i];
		if((buf>2100)&&(buf<2450))
		{
			data_code_lo+=1;
		}
	}

	if(data_code_hi==(uint8_t)~data_code_lo)
	{
		return 0;	 // 正确接收
	}
	else return 1;
}



