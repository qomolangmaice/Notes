#include "test_module.h"

static uint32 cout_char_len(const uint8* str, uint8* p_char_pos);

uint32 cout_char_len(const uint8* str, uint8* p_char_pos)
{
    uint32 u32_num = 0;
    uint8* tmp = str;
    
    if (NULL == str || NULL == p_char_pos)
    {
        printf("str or p_char_pos is NULL.\n");
        return 0;
    }
    
    while(tmp++ != p_char_pos)
        u32_num++;
       
    return (u32_num + 1);
}

char* merge_str(uint8* cmd)
{
    const uint8* front_cmd = "ps -ef | grep ";
    const uint8* back_cmd = " | awk '{print $2}' | xargs kill -9";
    
    if (NULL == cmd)
    {
        printf("cmd is NULL.\n");
        return NULL;
    }
    
    // 获取最后一个'/'出现的地址
    uint8* slash_pos = strrchr(cmd, '/');  
    // 最后一个'/'地址之后的字符串长度
    uint32 dst_cmd_len = strlen(cmd) - cout_char_len(cmd, slash_pos);   
    uint8 dst_cmd[32] = {0};
    strncpy(dst_cmd, slash_pos + 1, dst_cmd_len);
    dst_cmd[dst_cmd_len] = '\0';
    
    int merge_str_len = strlen(front_cmd) + strlen(back_cmd) + dst_cmd_len;
    
    uint8* merge_str = (uint8*)malloc(sizeof(uint8) * (merge_str_len + 2));
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

void open(uint8* cmd)
{
    system(cmd);
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
    const uint8* cmd = "/usr/bin/ls";
    uint8* slash_pos = strrchr(cmd, '/');
    
    uint32 num = cout_char_len(cmd, slash_pos);
    printf("test_cout_char_len -- num = %d\n", num);
}

void test_merge_str(void)
{
    uint8* cmd = "/usr/bin/deepin-terminal";
    uint8* tmp = merge_str(cmd);
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

