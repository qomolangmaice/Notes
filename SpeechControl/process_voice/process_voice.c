#include "process_voice.h"

#define ITEM_OF(array)  (int)(sizeof(array) / sizeof(array[0]))

#define MIN_BRIGHTNESS 0.5
#define MAX_BRIGHTNESS 1

#define TRUE  1
#define FALSE 0

const char *VOICE_AUTHORITY = "安迪";
const char *BRIGHTNESS = "亮度";

float g_brightness = 0.75;   // 亮度 0.1 ～ 10, 正常范围为0.1 ~ 1

typedef struct PROCESS_VOICE
{
   char* utterance;
   char* cmd;
   void (*p_process_func1)(char* cmd);
   void (*p_process_func2)(char* utterance, char* cmd);
}PROCESS_VOICE_T;

static void open_app(char* cmd);
static void close_app(char* cmd);
static void execute_cmd(char* cmd);
static void adjust_brightness(char* utterance, char *cmd);

static void text2speech(char* utterance);
static void str_trim_space(char* p_str);
static char* float2str(float val, int precision, char *buf);
static int cout_char_len(char* str, char* p_char_pos);
static char* merge_str(char* cmd);

PROCESS_VOICE_T g_process_voices[] =
{
  {"打开 终端", "deepin-terminal", open_app, NULL},
  {"关闭 终端", "deepin-terminal", close_app, NULL},
  {"打开 谷歌浏览器", "google-chrome", open_app, NULL},
  {"关闭 谷歌浏览器", "chrome", close_app, NULL},  // Can't close APP! FIX ME!!
  {"打开 WPS文字", "/opt/kingsoft/wps-office/office6/wps", open_app, NULL},
  {"关闭 WPS文字", "/opt/kingsoft/wps-office/office6/wps", close_app, NULL},
  {"打开 WPS表格", "/opt/kingsoft/wps-office/office6/et", open_app, NULL},
  {"关闭 WPS表格", "/opt/kingsoft/wps-office/office6/et", close_app, NULL},
  {"打开 WPS演示", "/opt/kingsoft/wps-office/office6/wpp", open_app, NULL},
  {"关闭 WPS演示", "/opt/kingsoft/wps-office/office6/wpp", close_app, NULL},
  {"打开 视频播放器", "deepin-movie", open_app, NULL},
  {"关闭 视频播放器", "deepin-movie", close_app, NULL},
  {"打开 音乐播放器", "deepin-music-player", open_app, NULL},
  {"关闭 音乐播放器", "deepin-music-player", close_app, NULL},
  {"打开 应用商店", "deepin-appstore", open_app, NULL},
  {"关闭 应用商店", "deepin-appstore", close_app, NULL},
  {"打开 有道词典", "youdao-dict", open_app, NULL},
  {"关闭 有道词典", "python3", close_app, NULL},  // 有道词典是使用python3打开的
  {"打开 文件管理器", "dde-file-manager", open_app, NULL},
  {"关闭 文件管理器", "dde-file-manager", close_app, NULL},    
  {"打开 图片浏览器", "deepin-image-viewer", open_app, NULL},
  {"关闭 图片浏览器", "deepin-image-viewer", close_app, NULL},
  {"打开 代码编辑器", "code", open_app, NULL},    // 打开 Visual Studio code
  {"关闭 代码编辑器", "code", close_app, NULL},   // 关闭 Visual Studio code
  {"打开 文本编辑器", "leafpad", open_app, NULL},    // 打开 Visual Studio code
  {"关闭 文本编辑器", "leafpad", close_app, NULL},   // 关闭 Visual Studio code
  {"打开 系统监视器", "gnome-system-monitor", open_app, NULL},
  {"关闭 系统监视器", "gnome-system-monitor", close_app, NULL},
  {"打开 日历", "dde-calendar", open_app, NULL},
  {"关闭 日历", "dde-calendar", close_app, NULL},

  {"关机", "shutdown -h now", execute_cmd, NULL},   
  {"重启", "shutdown -r now", execute_cmd, NULL}, 
  {"注销", "logout", execute_cmd, NULL},  
  {"设置 静音", "amixer sset Master mute", execute_cmd, NULL},     
  {"取消 静音", "amixer sset Master unmute", execute_cmd, NULL},
  {"加大 音量", "amixer -c 0 set PCM 10db+", execute_cmd, NULL},  // 将value值在原基础上加上10*2=20
  {"减小 音量", "amixer -c 0 set PCM 10db-", execute_cmd, NULL},  // 将value值在原基础上减去10*2=20

  {"加大 亮度", "xgamma -gamma ", NULL, adjust_brightness}, 
  {"减小 亮度", "xgamma -gamma ", NULL, adjust_brightness},

  {"放 一个 电影", "sh loop_and_random_play_movie.sh start", open_app, NULL},
  {"放 一首 歌", "sh loop_and_random_play_music.sh start", open_app, NULL},

  {"关闭 音乐", "play", close_app, NULL},
};

/*******************************************************************************
* Function Name  	: execute_cmd
* Description    	: 命令执行函数
* Input          	: cmd -- 待执行命令
* Output         	: None
* Return         	: None
*******************************************************************************/
static void execute_cmd(char* cmd)
{
    pid_t pid;
    int status;

    if(cmd == NULL)
    {
         //return (1);
         return;
    }
    
    pid = fork();
    if(pid < 0)     // 创建进程
    {
        status = -1;
    }
    else if(pid == 0)   // 执行子进程
    {
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
        _exit(127); //子进程正常执行则不会执行此语句
    }
    else    // 父进程什么也不做
    {
    /*
        while(waitpid(pid, &status, 0) < 0){
            if(errno != EINTER){
                status = -1;
                break;
            }
        }
        */
    }
    //return status;
}

/*******************************************************************************
* Function Name  	: float2str
* Description    	: 浮点型数据转换成字符串
* Input          	: val -- 浮点型数值
*                     precision -- 转换的数据长度
*                     buf -- 数据缓冲区
* Output         	: None
* Return         	: None
*******************************************************************************/
static char* float2str(float val, int precision, char *buf)
{
    char *cur, *end;
    
    sprintf(buf, "%.6f", val);
    if (precision < 6) {
        cur = buf + strlen(buf) - 1;
        end = cur - 6 + precision; 
        while ((cur > end) && (*cur == '0')) {
            *cur = '\0';
            cur--;
        }
    }
    
    return buf;
}

/*******************************************************************************
* Function Name  	: adjust_brightness
* Description    	: 调整屏幕的亮度
* Input          	: utterance -- 语音中提取的字符串
*                     cmd -- 与语音中提取的字符串对应的命令
* Output         	: None
* Return         	: None
*******************************************************************************/
static void adjust_brightness(char* utterance, char *cmd)
{
    char bri_buf[10] = {0};
    char *p = NULL;
    char total_cmd[128] = {0};

    if (g_brightness < MIN_BRIGHTNESS || 
        g_brightness > MAX_BRIGHTNESS)
    {
        printf("g_brightness is not in range.\n");
        g_brightness = 0.75;
        return;
    }

    if (strcmp(utterance, "加大 亮度") == 0)
    {
        g_brightness += 0.1;
        p = float2str((float)g_brightness, 2, bri_buf);
        sprintf(total_cmd, "%s%s", cmd, p);
        printf("total_cmd: %s\n", total_cmd);
        execute_cmd(total_cmd);
    }
    else if (strcmp(utterance, "减小 亮度") == 0)
    {
        g_brightness -= 0.1;
        p = float2str((float)g_brightness, 2, bri_buf);
        sprintf(total_cmd, "%s %s", cmd, p);
        printf("total_cmd: %s\n", total_cmd);
        execute_cmd(total_cmd);
    }
}

/*******************************************************************************
* Function Name  	: cout_char_len
* Description    	: 统计字符所在的位置的长度
* Input          	: str -- 字符串
*                     p_char_pos -- 字符在字符串所在的指针位置
* Output         	: None
* Return         	: None
*******************************************************************************/
static int cout_char_len(char* str, char* p_char_pos)
{
    int u32_num = 0;
    char* tmp = str;

    while(tmp++ != p_char_pos)
        u32_num++;
       
    return (u32_num + 1);
}

/*******************************************************************************
* Function Name  	: merge_str
* Description    	: 合并字符串
* Input          	: cmd -- 待合并的字符串
* Output         	: None
* Return         	: None
*******************************************************************************/
static char* merge_str(char* cmd)
{
    char* front_cmd = "ps -ef | grep ";
    char* back_cmd = NULL;

    if (NULL == cmd)
    {
        printf("cmd is NULL.\n");
        return (char*)0;
    }

    if (strcmp(cmd, "code") == 0 || 
        strcmp(cmd, "chrome") == 0)
    {
        // 杀掉所有相同程序名的多个进程
        back_cmd = " | awk '{print $2}' | xargs kill -9"; 
    }
    else
    {    
        // 杀掉相同程序名的最新打开的进程
        back_cmd = " | grep -v grep | cut -c 9-15 | tail -n 1 | xargs kill -9";  
    }
    
    // 获取最后一个'/'出现的地址
    char* slash_pos = NULL;
    slash_pos = strrchr(cmd, '/');
    int dst_cmd_len = 0;
    char dst_cmd[64] = {0};

    if (slash_pos == NULL || strncmp(cmd, "/opt", 4) == 0)
    {
        dst_cmd_len = strlen(cmd);           
        strncpy(dst_cmd, cmd, dst_cmd_len);
        printf("cmd: %s\n", cmd);
    }
    else
    {
        // 最后一个'/'地址之后的字符串长度
        dst_cmd_len = strlen(cmd) - cout_char_len(cmd, slash_pos);   
        strncpy(dst_cmd, slash_pos + 1, dst_cmd_len);
    }
    dst_cmd[dst_cmd_len] = '\0';
    
    int merge_str_len = strlen(front_cmd) + strlen(back_cmd) + dst_cmd_len;
    
    char* merge_str = (char*)malloc(sizeof(char) * (merge_str_len + 2));
    if (merge_str == NULL)
        printf("merge_str is NULL.\n");
        
    //printf("sizeof(merge_str) = %d\n", sizeof(merge_str));    // sizeof()不能求动态内存的长度，sizeof(merge_str)求的是指针的长度
    int num = snprintf(merge_str, merge_str_len + 1, "%s%s%s", front_cmd, dst_cmd, back_cmd);
    merge_str[merge_str_len] = '\0';
    //printf("num = %d, strlen(merge_str) = %d\n", num, strlen(merge_str));
    if (num > strlen(merge_str) || num < 0) // 只对 snprintf() 有效
    { 
        // merge_str的空间不足
        printf("The memory space of merge_str is not enough.\n");
    }
    
    return merge_str;
}

/*******************************************************************************
* Function Name  	: str_trim_space
* Description    	: 去除字符串中的空格
* Input          	: p_str -- 未去除空格的字符串
* Output         	: None
* Return         	: None
*******************************************************************************/
static void str_trim_space(char* p_str)  
{  
    char *p_tmp = p_str;  
      
    while (*p_str != '\0')   
    {  
        if (*p_str != ' ')  
        {  
            *p_tmp++ = *p_str;  
        }  
        ++p_str;  
    }  
    *p_tmp = '\0';  
} 

/*******************************************************************************
* Function Name  	: open_app
* Description    	: 打开应用程序
* Input          	: cmd -- 待打开应用程序的命令
* Output         	: None
* Return         	: None
*******************************************************************************/
static void open_app(char* cmd)
{
    execute_cmd(cmd);
}

/*******************************************************************************
* Function Name  	: close_app
* Description    	: 关闭应用程序
* Input          	: cmd -- 待关闭应用程序的命令
* Output         	: None
* Return         	: None
*******************************************************************************/
static void close_app(char* cmd)
{
    char* tmp = merge_str(cmd);
    execute_cmd(tmp);
    
    free(tmp);
}

/*******************************************************************************
* Function Name  	: text2speech
* Description    	: 文字转语音
* Input          	: text -- 待转换成语音的文字
* Output         	: None
* Return         	: None
*******************************************************************************/
static void text2speech(char* text)
{
    char buf[128] = {0};
    const char* ESPEAK_SAYS = "espeak -v zh+f3 -s 200 好的，正在帮你";
    //const char* ESPEAK_SAYS = "ekho -s 30 好的，正在帮你";
    int total_len = 0;
    char espeak_buf[128] = {0};
    int espeak_buf_len = 0;

    // 去除字符串中的空格
    espeak_buf_len = strlen(text);
    strncpy(espeak_buf, text, espeak_buf_len);
    espeak_buf[espeak_buf_len] = '\0';
    str_trim_space(&espeak_buf); 

    // 合并字符串
    total_len = strlen(ESPEAK_SAYS) + strlen(espeak_buf);
    snprintf(buf, total_len + 4, "%s%s", ESPEAK_SAYS, espeak_buf);
    buf[total_len + 4] = '\0';

    execute_cmd(buf);
}

/*******************************************************************************
* Function Name  	: send_notify
* Description    	: 桌面提醒
* Input          	: text -- 待转换成桌面提醒的文字
* Output         	: None
* Return         	: None
*******************************************************************************/
static void send_notify(char* text)
{
    char buf[128] = {0};
    const char* NOTIFY_TEXT = "notify-send 来自语音助手 好的，正在帮你";
    int total_len = 0;
    char notify_buf[128] = {0};
    int notify_buf_len = 0;

    // 去除字符串中的空格
    notify_buf_len = strlen(text);
    strncpy(notify_buf, text, notify_buf_len);
    notify_buf[notify_buf_len] = '\0';
    str_trim_space(&notify_buf); 

    // 合并字符串
    total_len = strlen(NOTIFY_TEXT) + strlen(notify_buf);
    snprintf(buf, total_len + 4, "%s%s", NOTIFY_TEXT, notify_buf);
    buf[total_len + 4] = '\0';

    execute_cmd(buf);
}

/*******************************************************************************
* Function Name  	: process_voice
* Description    	: 处理语音识别后获得的字符串所包含的内容
* Input          	: hyp -- 处理语音识别后获得的字符串
* Output         	: None
* Return         	: None
*******************************************************************************/
void process_voice(const char* hyp)
{
    int i = 0;
    
    for(i = 0; i < ITEM_OF(g_process_voices); i++)
    {
        if (strncmp(hyp, VOICE_AUTHORITY, 4) == 0)  // 当语音识别的语句的头两个字为"安迪", 才继续执行
        {
            if (strstr(hyp, g_process_voices[i].utterance) != NULL)     //如果语音识别的语句中包含已定义的操作
            {
                // 文字转语音
                text2speech(g_process_voices[i].utterance);

                // 桌面提醒
                send_notify(g_process_voices[i].utterance);

                // 执行相应命令
                if (strstr(hyp, BRIGHTNESS) != NULL)    // 如果语音识别的语句中包含“亮度”二字
                {
                    g_process_voices[i].p_process_func2(g_process_voices[i].utterance, g_process_voices[i].cmd);
                }
                else
                {
                    g_process_voices[i].p_process_func1(g_process_voices[i].cmd);
                }
                break; 
            }
        }
    }
}
