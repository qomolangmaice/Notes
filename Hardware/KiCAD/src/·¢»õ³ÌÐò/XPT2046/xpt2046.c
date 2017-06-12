#include "xpt2046.h"

#define TP_CS_LOW GPIOB->BRR=(1<<12);
#define TP_CS_HIGH GPIOB->BSRR=(1<<12);

uint16_t X_Coord,Y_Coord;

void Touch_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* 配置SCK引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* 配置MISO引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* 配置MOSI引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* 配置CS引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* CS引脚拉高 */
  GPIO_SetBits(GPIOB, GPIO_Pin_12);

  /* 配置SPI2 */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  /* 允许SPI2  */
  SPI_Cmd(SPI2, ENABLE);
	
	// 初始化触摸屏中断引脚TP_PEN
	
	/*开启按键端口PD的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	/* 选择上拉输入模式 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

uint8_t SPI2_Communication(uint8_t TxData)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, TxData);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

/*************************************************/
/* 功    能：读取X轴或Y轴的ADC值				         */
/* 入口参数：CMD:命令                            */ 
/* 说    明：读1次                               */
/*************************************************/
uint16_t TP_Read_AD(uint8_t CMD)
{	  
	uint16_t NUMH,NUML;
	uint16_t Num; 

	TP_CS_LOW; // CS=0 开始SPI通信	 
	SPI2_Communication(CMD);
	NUMH=SPI2_Communication(0x00);
	NUML=SPI2_Communication(0x00);
	Num=(NUMH<<8)+NUML; 	
	Num>>=3;                // 获取12位有效值
	TP_CS_HIGH;  // CS=1 结束SPI通信	 
	return(Num);   
}

/*************************************************/
/* 功    能：读取X轴或Y轴的ADC值				         */
/* 入口参数：CMD_RDX:读取X的ADC值                */ 
/*           CMD_RDY:读取Y的ADC值                */
/* 说    明：读10次，排序，取buf[5]作为最终值    */
/*************************************************/
uint16_t TP_Read_TenAD(uint8_t CMD)
{
	uint16_t i, j;
	uint16_t buf[5];
	uint16_t temp;
	
	for(i=0;i<5;i++)
	{				 
		buf[i]=TP_Read_AD(CMD);	    
	}				    
	for(i=0;i<5; i++)//排序
	{
		for(j=i+1;j<5;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	if((buf[4]-buf[0])>50)return 1;
	return buf[2];   
} 

/*************************************************/
/* 功能：把读出的ADC值转换成坐标值               */
/*************************************************/	  
uint8_t TP_Read_XY(void)
{
	uint16_t x,y;
	
	x = TP_Read_TenAD(CMD_RDX);
	y = TP_Read_TenAD(CMD_RDY);
	if(x<120||y<200)return 1; // 返回错误
	X_Coord = (240-(x-120)/16.167); // 把读到的X_ADC值转换成TFT X坐标值
	Y_Coord = (320-(y-200)/11.875); // 把读到的Y_ADC值转换成TFT Y坐标值 
	return 0; // 返回成功
}
	


