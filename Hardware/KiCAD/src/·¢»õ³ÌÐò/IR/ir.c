#include "ir.h"

volatile uint8_t pulse_bnum=0;		 // ���������
volatile uint16_t ir_buf[68];		 // װ���û����������
uint8_t custom_code_hi;
uint8_t custom_code_lo;
uint8_t data_code_hi;
uint8_t data_code_lo;

void IE_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;// ����һ��GPIO_InitTypeDef���͵ı���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// ����GPIOBʱ��
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1; // ѡ��PIN1
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; // ͨ���������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz�ٶ�
	GPIO_Init(GPIOB, &GPIO_InitStruct);// �Ѳ���������������
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
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
	// 560us�ߵ�ƽ
	for(i=0;i<21;i++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay17_53us();
	}
	//  560us�͵�ƽ
	for(i=0;i<21;i++)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay17_53us();
	}
}

void IE_Send_one()
{
	uint8_t i;
	// 560us�ߵ�ƽ
	for(i=0;i<21;i++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay17_53us();
	}
	//  1680us�͵�ƽ
	for(i=0;i<64;i++)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
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
	
	// ����9ms�ߵ�ƽ
	for(i=0;i<342;i++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay17_53us();
	}
	// ����4.5ms�͵�ƽ
	for(i=0;i<171;i++)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay8_77us();
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);//��GPIOB1��������
		delay17_53us();
	}
	// ���Ͷ�������ֽ�
	IE_Send_Data(custom_code_hi);
	// ���Ͷ�������ֽ�
	IE_Send_Data(custom_code_lo);
	// ����������
	IE_Send_Data(data_code);
	// ���������뷴��
	IE_Send_Data(~data_code);
	// ����ֹͣλ
	IE_Send_zero();
}

void IR_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStruct; 
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	/* �������� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);												
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	// ��������ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStruct);	

	/* ���������ж�ģʽ */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0); // PB0
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //˫�����ж�
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
  EXTI_Init(&EXTI_InitStruct);
	
	 /* ����NVIC */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;// ��Ӧ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; 
  NVIC_Init(&NVIC_InitStruct);
	
	/* ���ö�ʱ��6ÿ20ms�ж� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);//����ʱ��6
	TIM_TimeBaseInitStruct.TIM_Prescaler= 71;
	TIM_TimeBaseInitStruct.TIM_Period=20000;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	TIM_ClearFlag(TIM6, TIM_IT_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;	  
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;	// ��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/* �򿪶�ʱ��6 */
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
		return 0;	 // ��ȷ����
	}
	else return 1;
}



