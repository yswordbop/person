#include <stdio.h>

/**删除所有**/
char *pubf_str_rm(char *str, char *rm_str)
{
        if(*str == '\0') {
                return(NULL);
        }
        int old_idx = strcspn(str, rm_str);
        int new_idx = old_idx;
        int len = strlen(str);
        /*
        old_idx为原字符串索引，new_idx为新字符串索引;
        old_idx从左向右移动，遇rm_str跳过;
        否则，old_idx处值存入new_idx处，old_idx和new_idx同步+1
        */
        for (; old_idx < len; ++new_idx, ++old_idx)
        {
                old_idx += strspn(str + old_idx, rm_str);
                str[new_idx] = str[old_idx];
        }
        memset(str + new_idx, '\0', len - new_idx);
        return str;
}
