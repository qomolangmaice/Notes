#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uint8;

#define ITEM_OF(array)	    (int)(sizeof(array) / sizeof(array[0]))

typedef struct PROCESS_VOICE
{
   uint8* utterance;
   uint8* cmd;
   void (*p_process_func)(uint8* cmd);
}PROCESS_VOICE_T;

const PROCESS_VOICE_T s_process_voice[] = 
{
  {"打开 终端", "/usr/share/deepin-terminal/main", process_open},
  {"关闭 终端", "/usr/share/deepin-terminal/main", process_close}
};

static void test_strcat(void);
static void test_substring(uint8* src, uint8** dest);
static void process_open(uint8* cmd);
static void process_close(uint8* cmd);

void test_strcat(void)
// char* test_strcat(uint8* cmd)
{
    const uint8* front_cmd = "ps -ef | grep ";
    const uint8* back_end = " | awk '{print $2}' | xargs kill -9";
    uint8 cmd[20] = "/usr/bin/ls";
    uint8* dst_cmd = (uint8*)malloc(20);
    
    test_substring(cmd, &dst_cmd);
    
    int dest_len = sizeof(front_cmd) + sizeof(back_cmd) + sizeof(dst_cmd);
    
    uint8* dest = (uint8*)malloc(dest_len);
    
    int num = snprintf(dest, sizeof(dest), "%s%s%s", front_cmd, dst_cmd, back_cmd);
    assert(n < 0); // 这个format应该不可能出现
    if (n >= sizeof(dest)) { // 只对 snprintf() 有效
        // dest的空间不足
    }

    printf("dest: %s\n", dest);
    free(dest);
    free(dst_cmd);
}

void test_substring(uint8* src, uint8** dest)
{
    int pos = 0;
    
    for(int i = 0; i < sizeof(src); i++)
    {
        if (strcmp("/", src[i]) == 0)
        {
            pos++;
        }
    }
    
    for(i = pos; i < sizeof(src) - pos; i++)
        strcpy(*dst[i], str[i]);
}

void process_open(uint8* cmd)
{
    system(cmd);
}

void process_close(uint8* cmd)
{
    const uint8* front_cmd = "ps -ef | grep ";
    const uint8* back_end = " | awk '{print $2}' | xargs kill -9";
    uint8* tmp = test_strcat(cmd);
    system(tmp);
}

uin8 process_voice(uint8* hyp)
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
