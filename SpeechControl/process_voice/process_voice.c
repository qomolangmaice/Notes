#include "process_voice.h"

typedef struct PROCESS_VOICE
{
   uint8* utterance;                    // 语音识别后获得的语句
   uint8* cmd;                          // 系统命令
   void (*p_process_func)(uint8* cmd);  // 处理系统命令的函数
}PROCESS_VOICE_T;

//static void test_strcat(void);
//static void test_substring(uint8* src, uint8** dest);
static void process_open(uint8* cmd);
//static void process_close(uint8* cmd);

const PROCESS_VOICE_T s_process_voice[] = 
{
  {"打开 终端", "/usr/share/deepin-terminal/main", process_open}
  //{"关闭 终端", "/usr/share/deepin-terminal/main", process_close}
};

void process_open(uint8* cmd)
{
    system(cmd);
}

/*******************************************************************************
* Function Name  	: process_voice
* Description    	: 处理语音识别后获得的字符串所包含的内容
* Input          	: hyp -- 处理语音识别后获得的字符串
* Output         	: None
* Return         	: None
*******************************************************************************/
void process_voice(uint8* hyp)
{
    //pthread_t *t;
    
    //pthread_create(p_process_func ...);
    //pthread_join(p_process_func ...);
    
    //play_warning_tone();
    
    for(int i = 0; i < ITEM_OF(s_process_voice); i++)
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
    process_voice("打开 终端");
    
    return 0;
}


