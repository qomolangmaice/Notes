/****************************************************************************
File name: 	DynMemManage.h
Author: 	     
Version: 	 V1.0	Date: 
Description:  北向Modbus消息内存管理函数:
              (1).初始化创建一个"动态内存"大内存块.
              (2).申请"动态内存"块.
              (3).释放"动态内存"块.
Others: 		 无
History:
****************************************************************************/
#ifndef __DYNMEMORYMANAGE_H__  
#define __DYNMEMORYMANAGE_H__  
  
#include "sys_types.h"

/************************* 对外提供函数声明 *****************/
/**
 * @ingroup     北向Modbus消息内存管理
 * @brief       初始化创建一个的"动态内存"大内存块.
 *
 * @details 
 * @par 描述： 【应用场景】北向Modbus通信初始化
 *            
 * @attention 
 *             【注意事项】无
 *             【依赖关系说明】无
 *             【时序说明】无
 *             【兼容性】无
 *             【扩展性】无
 * @param [IN]  pMem -- 初始化创建一个的"动态内存"大内存块的首地址
 *              size -- 初始化创建一个的"动态内存"大内存块的大小
 * @param [OUT] 无
 * @retval  无
 * @author  
 * @date 2017-03-04 17:35
 */
extern void DynMemCreate(void *pMem, u32 size);  

/**
 * @ingroup     北向Modbus消息内存管理
 * @brief       申请"动态内存"块.
 *
 * @details 
 * @par 描述： 【应用场景】
 *            
 * @attention 
 *             【注意事项】无
 *             【依赖关系说明】无
 *             【时序说明】无
 *             【兼容性】无
 *             【扩展性】无
 * @param [IN]  size -- 申请"动态内存"块的大小
 * @param [OUT] 无
 * @retval      返回申请到的内存地址，若返回空，则申请失败 
 * @author  
 * @date 2017-03-04 17:35
 */
extern void *DynMemGet(u32 size);  

/**
 * @ingroup     北向Modbus消息内存管理
 * @brief       释放"动态内存"块.(注意此处"释放"并非指真正释放内存)
 *
 * @details 
 * @par 描述： 【应用场景】
 *            
 * @attention 
 *             【注意事项】无
 *             【依赖关系说明】无
 *             【时序说明】无
 *             【兼容性】无
 *             【扩展性】无
 * @param [IN]  pMem -- 需要释放的"动态内存"块的指针
 * @param [OUT] 无
 * @retval      
 * @author  
 * @date 2017-03-04 17:35
 */ 
extern void DynMemFree(void *pMem);  
  
#endif //__DYNMEMORYMANAGE_H__ 
