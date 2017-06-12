#ifndef __IR_H
#define __IR_H



#include "stm32f10x.h"


void IE_Init(void);
void IE_Send_one(void);
void IE_T(uint8_t custom_code_hi, uint8_t custom_code_lo, uint8_t data_code);
void IR_Init(void);
uint8_t ir_process(void);





#endif


