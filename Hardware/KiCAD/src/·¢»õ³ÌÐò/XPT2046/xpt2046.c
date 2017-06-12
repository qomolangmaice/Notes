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
	
	/* ����SCK���� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* ����MISO���� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* ����MOSI���� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* ����CS���� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* CS�������� */
  GPIO_SetBits(GPIOB, GPIO_Pin_12);

  /* ����SPI2 */
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

  /* ����SPI2  */
  SPI_Cmd(SPI2, ENABLE);
	
	// ��ʼ���������ж�����TP_PEN
	
	/*���������˿�PD��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	/* ѡ����������ģʽ */
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
/* ��    �ܣ���ȡX���Y���ADCֵ				         */
/* ��ڲ�����CMD:����                            */ 
/* ˵    ������1��                               */
/*************************************************/
uint16_t TP_Read_AD(uint8_t CMD)
{	  
	uint16_t NUMH,NUML;
	uint16_t Num; 

	TP_CS_LOW; // CS=0 ��ʼSPIͨ��	 
	SPI2_Communication(CMD);
	NUMH=SPI2_Communication(0x00);
	NUML=SPI2_Communication(0x00);
	Num=(NUMH<<8)+NUML; 	
	Num>>=3;                // ��ȡ12λ��Чֵ
	TP_CS_HIGH;  // CS=1 ����SPIͨ��	 
	return(Num);   
}

/*************************************************/
/* ��    �ܣ���ȡX���Y���ADCֵ				         */
/* ��ڲ�����CMD_RDX:��ȡX��ADCֵ                */ 
/*           CMD_RDY:��ȡY��ADCֵ                */
/* ˵    ������10�Σ�����ȡbuf[5]��Ϊ����ֵ    */
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
	for(i=0;i<5; i++)//����
	{
		for(j=i+1;j<5;j++)
		{
			if(buf[i]>buf[j])//��������
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
/* ���ܣ��Ѷ�����ADCֵת��������ֵ               */
/*************************************************/	  
uint8_t TP_Read_XY(void)
{
	uint16_t x,y;
	
	x = TP_Read_TenAD(CMD_RDX);
	y = TP_Read_TenAD(CMD_RDY);
	if(x<120||y<200)return 1; // ���ش���
	X_Coord = (240-(x-120)/16.167); // �Ѷ�����X_ADCֵת����TFT X����ֵ
	Y_Coord = (320-(y-200)/11.875); // �Ѷ�����Y_ADCֵת����TFT Y����ֵ 
	return 0; // ���سɹ�
}
	


