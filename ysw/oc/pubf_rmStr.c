#include <stdio.h>

/**ɾ������**/
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
        old_idxΪԭ�ַ���������new_idxΪ���ַ�������;
        old_idx���������ƶ�����rm_str����;
        ����old_idx��ֵ����new_idx����old_idx��new_idxͬ��+1
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

/**ֻɾ��һ��**/
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
