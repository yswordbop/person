#include <stdio.h>                                                                                            
#include "cJSON.h"

/************************************************************************
 * 函 数 名：pubf_str_idx
 * 函数功能：取出src_str中以sp_str为分隔符的第sub_idx子域的内容，放入sub_str
 * 描    述："abc|123|def|456"
 *            0   1   2   3
 * 返 回 值：处理后的字符串
************************************************************************/
char *pubf_str_idx(char *src_str, char *sp_str, int sub_idx, char *sub_str)
{
        static char *cur_str = 0;
        static char *cur_pos = 0;
        static  int  cur_idx = 0;

        if (src_str != cur_str || sub_idx != cur_idx)
        {
                /*未按顺序取字符串子域*/
                cur_str = src_str;
                cur_pos = src_str;
                cur_idx = 0;
        }

        for (int i = cur_idx; i < sub_idx; ++i)
        {
                /*找到索引子域的开头*/
                cur_pos += strcspn(cur_pos, sp_str) + 1;
        }

        int sub_len = strcspn(cur_pos, sp_str);
        memcpy(sub_str, cur_pos, sub_len);
        sub_str[sub_len] = '\0';

        cur_idx  = sub_idx + 1;
        cur_pos += sub_len + 1;

        return sub_str;
}

int nCJSONCheckTitleIsArray(char *psTitle)
{
        char    sTemp[32] = {0x00};
        char    sArrayNumber[32] = {0x00};
        pubf_str_idx(psTitle,"(",1,sTemp);

        if(strlen(sTemp)) {
                pubf_str_idx(sTemp,")",0,sArrayNumber);
                memset(psTitle+(strlen(psTitle)-strlen(sTemp) -1),0x00,strlen(psTitle)-strlen(sTemp)+1);
        }
        else {
                return -1;
        }

        if(strlen(sArrayNumber)) {
                return atoi(sArrayNumber);
        }
        else {
                return -1;
        }
}

int main()
{
  
    printf("%d\n", nCJSONCheckTitleIsArray("a(1).b(2)"));
    return 0;
}
