#include "process_voice.h"
#include "test_module.h"

typedef struct PROCESS_VOICE
{
   uint8* utterance;
   uint8* cmd;
   void (*p_process_func)(uint8* cmd);
}PROCESS_VOICE_T;

static void process_open(uint8* cmd);
static void process_close(uint8* cmd);

const PROCESS_VOICE_T s_process_voice[] = 
{
  {"打开 终端", "/usr/bin/gnome-terminal", process_open},
  {"关闭 终端", "/usr/bin/gnome-terminal", process_close}
};

void process_open(uint8* cmd)
{
    system(cmd);
}

void process_close(uint8* cmd)
{
    uint8* tmp = merge_str(cmd);
    system(tmp);
    
    free(tmp);
}

void process_voice(uint8* hyp)
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
    process_voice("打开 终端");
    process_voice("关闭 终端");
    
    return 0;
}
