#ifndef __ILI9341_H
#define __ILI9341_H

#include "stm32f10x.h"

/* дTFT�Ĵ�����ַ */
#define LCD_WR_REG(add)    ((*(volatile u16 *)((uint32_t)0x60000000)) = ((u16)add))
/* ��TFT�Ĵ���д���� */
#define LCD_WR_DATA(val)       ((*(volatile u16 *) ((uint32_t)0x60020000)) = ((u16)(val)))
	
#define     RED          0XF800	  //��ɫ
#define     GREEN        0X07E0	  //��ɫ
#define     BLUE         0X001F	  //��ɫ
#define     WHITE        0XFFFF	  //��ɫ
#define     BLACK        0X0000	  //��ɫ
#define     YELLOW       0XFFE0	  //��ɫ
#define     ORANGE       0XFC08	  //��ɫ
#define     GRAY  	     0X8430   //��ɫ
#define     LGRAY        0XC618	  //ǳ��ɫ
#define     DARKGRAY     0X8410	  //���ɫ
#define     PORPO        0X801F	  //��ɫ
#define     PINK         0XF81F	  //�ۺ�ɫ
#define     GRAYBLUE     0X5458   //����ɫ
#define     LGRAYBLUE    0XA651   //ǳ����ɫ
#define     DARKBLUE     0X01CF	  //�����ɫ
#define 		LIGHTBLUE    0X7D7C	  //ǳ��ɫ


extern uint16_t  POINT_COLOR;
extern uint16_t  BACK_COLOR;

void ILI9341_Iint(void);
void LCD_Clear(uint16_t color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t num);
void LCD_XYRAM(uint16_t stx,uint16_t sty,uint16_t endx,uint16_t endy);
void LCD_ShowNum(uint8_t x,uint16_t y,uint32_t num,uint8_t len);


#endif




