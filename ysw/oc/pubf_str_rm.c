#include <stdio.h>

/**ɾ������**/
char *pubf_str_rm(char *str, char *rm_str)
{
        if(*str == '\0') {
                return(NULL);
        }
        int old_idx = strcspn(str, rm_str);
        int new_idx = old_idx;
        int len = strlen(str);
        /*
        old_idxΪԭ�ַ���������new_idxΪ���ַ�������;
        old_idx���������ƶ�����rm_str����;
        ����old_idx��ֵ����new_idx����old_idx��new_idxͬ��+1
        */
        for (; old_idx < len; ++new_idx, ++old_idx)
        {
                old_idx += strspn(str + old_idx, rm_str);
                str[new_idx] = str[old_idx];
        }
        memset(str + new_idx, '\0', len - new_idx);
        return str;
}
