/****************************************************************************
File name: 	ModbusMsgQueue.c
Author: 	 
Version: 	 V1.0	Date: 
Description:  北向Modbus消息队列执行函数:
              (1).消息队列初始化.
              (2).消息入队列.
              (3).消息出队列.
              (4).判断消息队列是否为空.
              (5).打印消息队列内容.
Others: 		 无
History:
****************************************************************************/

/*********************  系统级别的相关头文件 *******************/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

/*********************  bsp提供的相关头文件  ******************/


/*********************  内部声明的相关头文件 ******************/
#include "NorthModbusMsgQueue.h"
#include "DynMemManage.h"
#include "InnerUart.h"

/*******************************************************************************
* Function Name  	: NorthModbusMsgQueueInit
* Description    	: 北向Modbus消息队列初始化
* Input          	: pMdsMsgQueue -- 北向Modbus消息队列起始地址
* Output         	: None
* Return         	: None
* Author			: 
* Others			: None
********************************************************************************/
void NorthModbusMsgQueueInit(MDS_MSG_QUEUE_ST *pMdsMsgQueue)
{
	MDS_MSG_QUEUE_NODE_ST *pNode = (MDS_MSG_QUEUE_NODE_ST*)malloc(sizeof(MDS_MSG_QUEUE_NODE_ST));
	if (pNode == NULL)
	{
        DRV_Printf("NorthModbusMsgQueueInit malloc failed!\n");
        return ;
	}

	pNode->data.m_usMsgLenth= 0;
	pNode->data.m_pMsg = NULL;
	
	pMdsMsgQueue->head = pMdsMsgQueue->tail = pNode;
	pMdsMsgQueue->tail->next = NULL;
}

/*******************************************************************************
* Function Name  	: IsMdsMsgQueueEmpty
* Description    	: 判断北向Modbus消息队列是否为空
* Input          	: pMdsMsgQueue -- 北向Modbus消息队列起始地址
* Output         	: None
* Return         	: None
* Author			: 
* Others			: None
********************************************************************************/
BOOL IsMdsMsgQueueEmpty(MDS_MSG_QUEUE_ST *pMdsMsgQueue)
{
    return ((pMdsMsgQueue->head == pMdsMsgQueue->tail) ? TRUE : FALSE);
}

/*******************************************************************************
* Function Name  	: NorthModbus_In_MsgQueue
* Description    	: 北向Modbus消息入消息队列
* Input          	: pMdsMsgQueue -- 北向Modbus消息队列起始地址
*                 : pInQueueData -- 入消息队列的数据指针
*                 : u32InqueueDataSize -- 入消息队列的数据大小
* Output         	: None
* Return         	: None
* Author			: 
* Others			: None
********************************************************************************/
void NorthModbus_In_MsgQueue(MDS_MSG_QUEUE_ST *pMdsMsgQueue, u8 *pInQueueData, u32 u32InqueueDataSize)
{
	MDS_MSG_QUEUE_NODE_ST *pNewDataNode = (MDS_MSG_QUEUE_NODE_ST *)malloc(sizeof(MDS_MSG_QUEUE_NODE_ST)); 
	if (pNewDataNode == NULL)
	{
        DRV_Printf("NorthModbus_In_MsgQueue malloc failed!\n");
        return ;
	}

	// 获取消息的"动态内存"
	pNewDataNode->data.m_pMsg = (u8 *)DynMemGet(u32InqueueDataSize);
	if (pNewDataNode->data.m_pMsg == NULL)
	{
        DRV_Printf("NorthModbus_In_MsgQueue DynMemGet failed!\n");
        return ;
	}

	pNewDataNode->data.m_usMsgLenth = u32InqueueDataSize;
	(void)memcpy(pNewDataNode->data.m_pMsg, pInQueueData, u32InqueueDataSize);
	pNewDataNode->next = NULL;

	pMdsMsgQueue->tail->next = pNewDataNode;
	pMdsMsgQueue->tail = pNewDataNode;
}

/*******************************************************************************
* Function Name  	: NorthModbus_Out_MsgQueue
* Description    	: 北向Modbus消息出消息队列
* Input          	: pMdsMsgQueue -- 北向Modbus消息队列起始地址
*                 : pOutQueueData -- 出消息队列的数据节点指针
* Output         	: None
* Return         	: None
* Author			: 
* Others			: None
********************************************************************************/
void NorthModbus_Out_MsgQueue(MDS_MSG_QUEUE_ST *pMdsMsgQueue, MDS_MSG_QUEUE_NODE_ST *pOutQueueData)
{
	if(IsMdsMsgQueueEmpty(pMdsMsgQueue))
    {
        DRV_Printf("NorthModbus queue is empty!\n");
        return ;
    }

	MDS_MSG_QUEUE_NODE_ST *pTmp = pMdsMsgQueue->head->next;

	pOutQueueData->data.m_usMsgLenth = pTmp->data.m_usMsgLenth;
    (void)memcpy(pOutQueueData->data.m_pMsg, pTmp->data.m_pMsg, pTmp->data.m_usMsgLenth);

	pMdsMsgQueue->head->next = pTmp->next;
	
	free(pTmp);

	// 释放消息的"动态内存"
	DynMemFree(pTmp->data.m_pMsg);
	
	if(pTmp == pMdsMsgQueue->tail)
	{
		pMdsMsgQueue->tail = pMdsMsgQueue->head;
    }
}

/*******************************************************************************
* Function Name  	: NorthModbus_Show_MsgQueue
* Description    	: 打印北向Modbus消息队列内容
* Input          	: pMdsMsgQueue -- 北向Modbus消息队列起始地址
* Output         	: None
* Return         	: None
* Author			: 
* Others			: None
********************************************************************************/
void NorthModbus_Show_MsgQueue(MDS_MSG_QUEUE_ST *pMdsMsgQueue)
{
	MDS_MSG_QUEUE_NODE_ST *pTmp = pMdsMsgQueue->head->next;
    int i = 0;
	while(pTmp != NULL)
	{
        DRV_Printf("\nMsgQueue show: ");
        PrintFrameData(pTmp->data.m_usMsgLenth, pTmp->data.m_pMsg);
		pTmp = pTmp->next;
	}
}

