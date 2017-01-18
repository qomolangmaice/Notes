#ifndef __TEST_MODULE_H__
#define __TEST_MODULE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uint8;
typedef unsigned int uint32;

/************************* 对外提供函数声明 *****************/
/**
 * @ingroup		  语音识别处理模块
 * @brief       筛选字符串之后，再合并字符串
 *
 * @details 
 * @par 描述：  【应用场景】pocketsphinx处理语音识别
 *            
 * @attention 
 *             【注意事项】 筛选字符串之后，再合并字符串
 *             【依赖关系说明】无
 *             【时序说明】无
 *             【兼容性】无
 *             【扩展性】无
 * @param [IN]  cmd -- 待筛选字符串之后，再合并的字符串
 * @param [OUT] 无
 * @retval		  无				
 * @author  
 * @date 2017-01-18 12:12
 */
extern char* merge_str(uint8* cmd);

#endif
