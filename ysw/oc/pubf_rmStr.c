#include <stdio.h>

/**删除所有**/
/****
char *pubf_str_rm(char *str, char *rm_str)
{
        if(*str == '\0') {
                return(NULL);
        }
        int old_idx = strcspn(str, rm_str);
        int new_idx = old_idx;
        int len = strlen(str);
        / *
        old_idx为原字符串索引，new_idx为新字符串索引;
        old_idx从左向右移动，遇rm_str跳过;
        否则，old_idx处值存入new_idx处，old_idx和new_idx同步+1
        * /
        for (; old_idx < len; ++new_idx, ++old_idx)
        {
                old_idx += strspn(str + old_idx, rm_str);
                str[new_idx] = str[old_idx];
        }
        memset(str + new_idx, '\0', len - new_idx);
        return str;
}
**/

/**只删除一次**/
int pubf_rmStr(char *psSrc, char *psRm)
{
        char *p=strstr(psSrc,psRm);
    printf("p=[%s][%c]\n",p,*p);
        if(p){
                char    *pMsgBuff =  (char *)malloc(strlen(psSrc) + 1);
                memset(pMsgBuff,0x00,strlen(psSrc) + 1);
                memcpy(pMsgBuff,psSrc,strlen(psSrc)-strlen(p));
                memcpy(pMsgBuff+(strlen(psSrc)-strlen(p)),p+strlen(psRm),strlen(psSrc)-strlen(psRm));
                memset(psSrc,0x00,strlen(psSrc));
                memcpy(psSrc,pMsgBuff,strlen(pMsgBuff));
                free(pMsgBuff);
        }
        return 0;
}


main ()
{
    char *line="1|2|3|4|55|66";
    char *line1="1|2|3|4|55|66";
    int  index;
    char line_var[10];
    memset(line_var,0,sizeof(line_var));

    for(int i=0;i<9;i++)
   {
    index=pubf_rmStr(line,"|");
    printf("i=[%d] line_var=[%s] line=[%s]\n",i,index,line);
   }

    printf("line1_rm=[%s] \n",pubf_str_rm(line1,"|"));

    return 0 ;
}
