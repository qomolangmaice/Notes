#include "test_module.h"

static int cout_char_len(char* str, char* p_char_pos);

int cout_char_len(char* str, char* p_char_pos)
{
    int u32_num = 0;
    char* tmp = str;
    
    if (NULL == str || NULL == p_char_pos)
    {
        printf("str or p_char_pos is NULL.\n");
        return 0;
    }
    
    while(tmp++ != p_char_pos)
        u32_num++;
       
    return (u32_num + 1);
}

char* merge_str(char* cmd)
{
    char* front_cmd = "ps -ef | grep ";
    // 杀掉所有相同程序名的多个进程
    //const char* back_cmd = " | awk '{print $2}' | xargs kill -9"; 
    // 杀掉相同程序名的最新打开的进程
   char* back_cmd = " | grep -v grep | cut -c 9-15 | tail -n 1 | xargs kill -9";  
    
    if (NULL == cmd)
    {
        printf("cmd is NULL.\n");
        return (char*)0;
    }
    
    // 获取最后一个'/'出现的地址
    char* slash_pos = strrchr(cmd, '/');  
    // 最后一个'/'地址之后的字符串长度
    int dst_cmd_len = strlen(cmd) - cout_char_len(cmd, slash_pos);   
    char dst_cmd[32] = {0};
    strncpy(dst_cmd, slash_pos + 1, dst_cmd_len);
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

int execute_cmd(const char * cmdstring)
{
    pid_t pid;
    int status;

    if(cmdstring == NULL)
    {
         return (1);
    }
    
    if((pid = fork()) < 0)
    {
        status = -1;
    }
    else if(pid == 0)
    {
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127); //子进程正常执行则不会执行此语句
    }
    else
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
    return status;
}

void open(char* cmd)
{
    execute_cmd(cmd);
}

void test_strrchr(void)
{
    char *str = "/usr/bin/ls";
    char *tmp = NULL;
    tmp = strrchr(str, '/');
    
    printf("test_strrchr -- tmp: %s\n", tmp + 1);
}

void test_cout_char_len(void)
{
    char* cmd = "/usr/bin/ls";
    char* slash_pos = strrchr(cmd, '/');
    
    int num = cout_char_len(cmd, slash_pos);
    printf("test_cout_char_len -- num = %d\n", num);
}

void test_merge_str(void)
{
    char* cmd = "/usr/bin/deepin-terminal";
    char* tmp = merge_str(cmd);
    printf("test_merge_str -- tmp: %s\n", tmp);
    
    free(tmp);
}

/*
int main(int argc, char *argv[])
{
    test_strrchr();
    test_cout_char_len();
    test_merge_str();
    //open("/usr/bin/firefox 'www.cnblogs.com'");
    
    return 0;
}
*/

