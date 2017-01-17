#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
/*
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
*/

char* test_substring(uint8* src)
{
    int i = 0, pos = 0;
    uint8* dst = (uint8*)malloc(100);
    int pos_max = 0;
    
    for(i = 0; i < sizeof(src); i++)
    {
        if ('/' == src[i])
        {
            pos++;
            if(pos)
        }
    }
    printf("src: %s, pos = %d\n", src, pos);
    
    for(i = pos; i < sizeof(src) - pos; i++)
        memcpy(dst[i], src[i], sizeof(src) - pos);

    dst[i] = '\0';

    return dst;
}

int main(int argc, char *argv[])
{
    char* str = "/usr/bin/play";
    char *p;

    p = test_substring(str);

    printf("p: %s\n", p);

    free(p);

    return 0;
}

/*
char* substring(char* ch,int pos,int length)  
{  
    char* pch=ch;  
    //定义一个字符指针，指向传递进来的ch地址。  
    char* subch=(char*)calloc(sizeof(char),length+1);  
    //通过calloc来分配一个length长度的字符数组，返回的是字符指针。  
    int i;  
    //只有在C99下for循环中才可以声明变量，这里写在外面，提高兼容性。  
    pch=pch+pos;  
    //是pch指针指向pos位置。  
    for(i=0;i<length;i++)  
    {  
        subch[i]=*(pch++);  
    //循环遍历赋值数组。  
    }  
    subch[length]='\0';//加上字符串结束符。  
    return subch;       //返回分配的字符数组地址。  
} 

int main(){
    char* result;
    char* tstStr = "abcdefg";
    result = substring(tstStr,0,2);
    
    printf("结果：%s",result);
    
}
*/
