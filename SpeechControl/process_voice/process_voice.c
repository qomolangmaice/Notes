#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_module/test_module.h"

//typedef char uint8;

#define ITEM_OF(array)	    (int)(sizeof(array) / sizeof(array[0]))

typedef struct PROCESS_VOICE
{
   char* utterance;
   char* cmd;
   void (*p_process_func)(char* cmd);
}PROCESS_VOICE_T;

static void process_open(char* cmd);
static void process_close(char* cmd);

PROCESS_VOICE_T s_process_voice[] = 
{
  {"打开 终端", "/usr/bin/gnome-terminal", process_open},
  {"关闭 终端", "/usr/bin/gnome-terminal", process_close}
};

void process_open(char* cmd)
{
    execute_cmd(cmd);
}

void process_close(char* cmd)
{
    char* tmp = merge_str(cmd);
    execute_cmd(tmp);
    
    free(tmp);
}

/*******************************************************************************
* Function Name  	: process_voice
* Description    	: 处理语音识别后获得的字符串所包含的内容
* Input          	: hyp -- 处理语音识别后获得的字符串
* Output         	: None
* Return         	: None
*******************************************************************************/
void process_voice(char* hyp)
{
    int i = 0;
    //pthread_t *t;
    
    //pthread_create(p_process_func ...);
    //pthread_join(p_process_func ...);
    
    //play_warning_tone();
    
    for(i = 0; i < ITEM_OF(s_process_voice); i++)
    {
        if(strcmp(hyp, s_process_voice[i].utterance) == 0)
        {
            s_process_voice[i].p_process_func(s_process_voice[i].cmd);
            break;
        }
    }
}

int main(void)
{
    //process_voice("打开 终端");
    process_voice("关闭 终端");
    
    return 0;
}
