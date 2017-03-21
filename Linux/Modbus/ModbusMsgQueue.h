/****************************************************************************
File name: 	MsgQueue.h
Version: 	 V1.0	Date:
Description:  北向Modbus消息队列执行函数头文件:
              (1).消息队列初始化.
              (2).消息入队列.
              (3).消息出队列.
              (4).判断消息队列是否为空.
              (5).打印消息队列内容.
Others: 		无
History:
****************************************************************************/
#ifndef __MODBUSMSGQUEUE_H__
#define __MODBUSMSGQUEUE_H__

#include "sys_types.h"

// 北向Modbus消息抽象数据结构
typedef struct tagMdsMsg
{
    u16 m_usMsgLenth;            		    // 消息长度
    u8* m_pMsg;                             // 消息指针
} MDS_MSG_ST;

// 北向Modbus消息队列节点数据结构
typedef struct MDS_MSG_QUEUE_NODE 
{
 	MDS_MSG_ST data;                        // 数据域
	struct MDS_MSG_QUEUE_NODE *next;        // 指针域
} MDS_MSG_QUEUE_NODE_ST; 

// 北向Modbus消息队列数据结构
typedef struct 
{
	MDS_MSG_QUEUE_NODE_ST *head;            // 消息队列队头
	MDS_MSG_QUEUE_NODE_ST *tail;            // 消息队列队尾
} MDS_MSG_QUEUE_ST;

/**
 * @ingroup     北向Modbus消息队列
 * @brief       北向Modbus消息队列初始化
 *
 * @details 
 * @par 描述：【应用场景】北向Modbus消息队列初始化函数，大循环之前调用
 *            
 * @attention 
 *             【注意事项】无
 *             【依赖关系说明】无
 *             【时序说明】无
 *             【兼容性】无
 *             【扩展性】无
 * @param [IN]  pMdsMsgQueue -- 北向Modbus消息队列起始地址
 * @param [OUT] 无
 * @retval  无
 * @author  
 * @date 2017-03-02 11:40
 */
extern void NorthModbusMsgQueueInit(MDS_MSG_QUEUE_ST *pMdsMsgQueue);

/**
 * @ingroup     北向Modbus消息队列
 * @brief       判断北向Modbus消息队列是否为空
 *
 * @details 
 * @par 描述：【应用场景】判断北向Modbus消息队列是否为空函数，大循环中调用
 *            
 * @attention 
 *             【注意事项】无
 *             【依赖关系说明】无
 *             【时序说明】无
 *             【兼容性】无
 *             【扩展性】无
 * @param [IN]  pMdsMsgQueue -- 北向Modbus消息队列起始地址
 * @param [OUT] 无
 * @retval  无
 * @author  
 * @date 2017-03-02 11:40
 */
extern BOOL IsMdsMsgQueueEmpty(MDS_MSG_QUEUE_ST *pMdsMsgQueue);

/**
 * @ingroup     北向Modbus消息队列
 * @brief       北向Modbus消息入队列
 *
 * @details 
 * @par 描述：【应用场景】北向Modbus消息入队列函数，大循环中调用
 *            
 * @attention 
 *             【注意事项】无
 *             【依赖关系说明】无
 *             【时序说明】无
 *             【兼容性】无
 *             【扩展性】无
 * @param [IN]  pMdsMsgQueue -- 北向Modbus消息队列起始地址
 *              pInQueueData -- 入队列的消息数据指针
 *              u32InqueueDataSize -- 入队列的消息数据大小
 * @param [OUT] 无
 * @retval  无
 * @author  
 * @date 2017-03-02 11:40
 */
extern void NorthModbus_In_MsgQueue(MDS_MSG_QUEUE_ST *pMdsMsgQueue, u8 *pInQueueData, u32 u32InqueueDataSize);

/**
 * @ingroup     北向Modbus消息队列
 * @brief       北向Modbus消息出队列
 *
 * @details 
 * @par 描述：【应用场景】北向Modbus消息出队列函数，大循环中调用
 *            
 * @attention 
 *             【注意事项】无
 *             【依赖关系说明】无
 *             【时序说明】无
 *             【兼容性】无
 *             【扩展性】无
 * @param [IN]  pMdsMsgQueue -- 北向Modbus消息队列起始地址
 * @param [OUT] pOutQueueData -- 出队列的消息数据节点指针
 * @retval  无
 * @author  
 * @date 2017-03-02 11:40
 */
extern void NorthModbus_Out_MsgQueue(MDS_MSG_QUEUE_ST *pMdsMsgQueue, MDS_MSG_QUEUE_NODE_ST *pOutQueueData);

/**
 * @ingroup     北向Modbus消息队列
 * @brief       北向Modbus消息队列打印函数
 *
 * @details 
 * @par 描述：【应用场景】北向Modbus消息队列打印函数，调试中使用
 *            
 * @attention 
 *             【注意事项】无
 *             【依赖关系说明】无
 *             【时序说明】无
 *             【兼容性】无
 *             【扩展性】无
 * @param [IN]  pMdsMsgQueue -- 北向Modbus消息队列起始地址
 * @param [OUT] 无
 * @retval  无
 * @author  
 * @date 2017-03-02 11:40
 */
extern void NorthModbus_Show_MsgQueue(MDS_MSG_QUEUE_ST *pMdsMsgQueue);

#endif
