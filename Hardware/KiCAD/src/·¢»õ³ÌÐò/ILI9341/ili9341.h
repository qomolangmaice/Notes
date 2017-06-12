#ifndef __ILI9341_H
#define __ILI9341_H

#include "stm32f10x.h"

/* 写TFT寄存器地址 */
#define LCD_WR_REG(add)    ((*(volatile u16 *)((uint32_t)0x60000000)) = ((u16)add))
/* 给TFT寄存器写数据 */
#define LCD_WR_DATA(val)       ((*(volatile u16 *) ((uint32_t)0x60020000)) = ((u16)(val)))
	
#define     RED          0XF800	  //红色
#define     GREEN        0X07E0	  //绿色
#define     BLUE         0X001F	  //蓝色
#define     WHITE        0XFFFF	  //白色
#define     BLACK        0X0000	  //黑色
#define     YELLOW       0XFFE0	  //黄色
#define     ORANGE       0XFC08	  //橙色
#define     GRAY  	     0X8430   //灰色
#define     LGRAY        0XC618	  //浅灰色
#define     DARKGRAY     0X8410	  //深灰色
#define     PORPO        0X801F	  //紫色
#define     PINK         0XF81F	  //粉红色
#define     GRAYBLUE     0X5458   //灰蓝色
#define     LGRAYBLUE    0XA651   //浅灰蓝色
#define     DARKBLUE     0X01CF	  //深灰蓝色
#define 		LIGHTBLUE    0X7D7C	  //浅蓝色


extern uint16_t  POINT_COLOR;
extern uint16_t  BACK_COLOR;

void ILI9341_Iint(void);
void LCD_Clear(uint16_t color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t num);
void LCD_XYRAM(uint16_t stx,uint16_t sty,uint16_t endx,uint16_t endy);
void LCD_ShowNum(uint8_t x,uint16_t y,uint32_t num,uint8_t len);


#endif




