#ifndef __PROCESS_VOICE_H__
#define __PROCESS_VOICE_H__

/************ 系统库 **********/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/************ 本地公共库 **********/
#include "common.h"

/************************* 对外提供函数声明 *****************/
/**
 * @ingroup		  语音识别处理模块
 * @brief       处理语音识别后获得的字符串所包含的内容
 *
 * @details 
 * @par 描述：  【应用场景】pocketsphinx处理语音识别
 *            
 * @attention 
 *             【注意事项】处理语音识别后获得的字符串所包含的内容
 *             【依赖关系说明】无
 *             【时序说明】无
 *             【兼容性】无
 *             【扩展性】无
 * @param [IN]  hyp -- 处理语音识别后获得的字符串
 * @param [OUT] 无
 * @retval		  无				
 * @author  
 * @date 2017-01-18 12:12
 */
extern void process_voice(uint8* hyp);

#endif
