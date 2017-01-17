#ifndef __PROCESS_VOICE_H__
#define __PROCESS_VOICE_H__

/************ 系统库 **********/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/************ 本地公共库 **********/
#include "common.h"

/*******************************************************************************
* Function Name  	: process_voice
* Description    	: 处理语音识别后获得的字符串所包含的内容
* Input          	: hyp -- 处理语音识别后获得的字符串
* Output         	: None
* Return         	: None
*******************************************************************************/
extern void process_voice(uint8* hyp);

#endif
