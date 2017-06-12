#ifndef __XPT2046_H__
#define __XPT2046_H__
#include "stm32f10x.h"

extern uint16_t X_Coord,Y_Coord;

#define CMD_RDY 0X90  //0B10010000即用差分方式读X坐标
#define CMD_RDX	0XD0  //0B11010000即用差分方式读Y坐标   

extern void Touch_Init(void);
extern uint16_t TP_Read_AD(uint8_t CMD);
extern uint16_t TP_Read_TenAD(uint8_t CMD);
extern uint8_t TP_Read_XY(void);

#endif

