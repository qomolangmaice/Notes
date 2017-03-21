/****************************************************************************
File name: 	DynMemManage.c
Author: 	   
Version: 	 V1.0	Date:
Description:  北向Modbus消息内存管理函数:
              (1).初始化创建一个"动态内存"大内存块.
              (2).申请"动态内存"块.
              (3).释放"动态内存"块.
Others: 		 无
History:
****************************************************************************/
#include "stdio.h"  
#include "string.h"  
#include "DynMemManage.h" 

typedef enum  
{  
    DYNMEM_FREE,        // "动态内存"为空闲状态
    DYNMEM_USED,        // "动态内存"为已使用状态
} DYNMEM_USE_STATE;

typedef struct  
{  
    DYNMEM_USE_STATE used;      // "动态内存"状态
    u32 size;                   // "动态内存"的大小
    void *pPrev;                // "动态内存"的前驱指针
    void *pNext;                // "动态内存"的后驱指针
} DYN_MEM_TYPE_ST;  

static DYN_MEM_TYPE_ST *pDynMem = NULL;

/*******************************************************************************
* Function Name  	: DynMemCreate
* Description    	: 初始化创建一个"动态内存"大内存块.
* Input          	: pMem -- 初始化创建一个的"动态内存"大内存块的首地址
*                 : size -- 初始化创建一个的"动态内存"大内存块的大小
* Output         	: None
* Return         	: None
* Author			: 
* Others			: None
********************************************************************************/
void DynMemCreate(void *pMem, u32 size)  
{  
    if(NULL == pMem)  
        return;  
    if(size <= sizeof(DYN_MEM_TYPE_ST))  
        return ;  
      
    pDynMem = pMem;  
    pDynMem->used = DYNMEM_FREE;  
    pDynMem->size = size - sizeof(DYN_MEM_TYPE_ST);  
    pDynMem->pPrev = NULL;  
    pDynMem->pNext = NULL;  
}  
  
/*******************************************************************************
* Function Name  	: DynMemGet
* Description    	: 申请"动态内存"块.
* Input          	: size -- 申请"动态内存"块的大小
* Output         	: None
* Return         	: 返回申请到的内存地址(空指针类型)，若返回空，则申请失败 
* Author			: 
* Others			: None
********************************************************************************/
void *DynMemGet(u32 size)  
{  
    DYN_MEM_TYPE_ST *pGet = pDynMem;    // 申请的内存块指针
    DYN_MEM_TYPE_ST *pFree = NULL;      // 空闲的内存块指针
      
    if(size == 0)  
        return NULL;  
      
    size = ((size + 3) / 4) * 4;  // size大小按4的倍数来分配  
      
    while(pGet != NULL)  
    {   //找到合适空间  
        if((pGet->used != DYNMEM_FREE) || (pGet->size < size))  
        {  
            pGet = pGet->pNext;  
            continue;  
        }  
          
        //找到空闲且大小合适的内存区域  
        if(pGet->size - size > sizeof(DYN_MEM_TYPE_ST))  
        {//可以放下一个新的内存块表，将其一分为二  
            pFree = (DYN_MEM_TYPE_ST*)((u32)pGet + sizeof(DYN_MEM_TYPE_ST) + size);
              
            // 修改链表关系
            // Step 1 
            pFree->pPrev = pGet;  
            
            // Step 2 
            pFree->pNext = pGet->pNext;  
            
            // Step 3 
            pGet->pNext = pFree;  
            
            // Step 4 
            if(NULL != pFree->pNext)  
            {  
                (*(DYN_MEM_TYPE_ST*)(pFree->pNext)).pPrev = pFree;  
            }  
              
            // 剩余空间配置
            pFree->used = DYNMEM_FREE;  
            pFree->size = pGet->size - size - sizeof(DYN_MEM_TYPE_ST);  
            // 获取到的空间配置 
            pGet->used = DYNMEM_USED;  
            pGet->size = size;  
        }  
        else  
        {//已经放不下一个新的内存块表，将其全部占有  
            pGet->used = DYNMEM_USED;  
        }  
          
        return (void*)((u32)pGet + sizeof(DYN_MEM_TYPE_ST));  
    }  
      
    return NULL;  
}  

/*******************************************************************************
* Function Name  	: DynMemFree
* Description    	: 释放"动态内存"块(注意此处"释放"并非指真正释放内存).
* Input          	: size -- 释放"动态内存"块的大小
* Output         	: None
* Return         	: None
* Author			: 
* Others			: None
********************************************************************************/
void DynMemFree(void *pMem)  
{  
    DYN_MEM_TYPE_ST *pPut = NULL;  
      
    if(NULL == pMem)  
        return;  
      
    pPut = (DYN_MEM_TYPE_ST*)((u32)pMem - sizeof(DYN_MEM_TYPE_ST));  // 释放的内存块指针,表示将内存归还给"动态内存"大内存块
    pPut->used = DYNMEM_FREE;  
      
    //释放空间，整体释放/合二为一/合三为一  
    if((pPut->pPrev != NULL) && (((DYN_MEM_TYPE_ST*)(pPut->pPrev))->used == DYNMEM_FREE))  
    {  
        ((DYN_MEM_TYPE_ST*)(pPut->pPrev))->size += sizeof(DYN_MEM_TYPE_ST) + pPut->size;  
        ((DYN_MEM_TYPE_ST*)(pPut->pPrev))->pNext = pPut->pNext;  
          
        if(pPut->pNext != NULL)  
        {  
            ((DYN_MEM_TYPE_ST*)(pPut->pNext))->pPrev = pPut->pPrev;  
        }  
          
        pPut = (DYN_MEM_TYPE_ST*)(pPut->pPrev);  
    }  
    if((pPut->pNext != NULL) && (((DYN_MEM_TYPE_ST*)(pPut->pNext))->used == DYNMEM_FREE))  
    {  
        pPut->size += sizeof(DYN_MEM_TYPE_ST) + ((DYN_MEM_TYPE_ST*)(pPut->pNext))->size;  
        pPut->pNext = ((DYN_MEM_TYPE_ST*)(pPut->pNext))->pNext;  
          
        if(pPut->pNext != NULL)  
        {  
            ((DYN_MEM_TYPE_ST*)(pPut->pNext))->pPrev = pPut;  
        }  
    }  
}  
