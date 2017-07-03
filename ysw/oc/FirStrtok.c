#include <stdio.h>


char* FirStrtok(char* instr, char* outstr, char* delimiter)
{
        char* tmp;
        if(instr == NULL || strlen(instr) == 0 || delimiter == NULL)
                return NULL;
        tmp = strstr(instr,delimiter);
        if(tmp != NULL)
        {
                memcpy(outstr,instr,strlen(instr)-strlen(tmp));
                return (tmp + strlen(delimiter));
        }
        return NULL;
}

int main()
{
    char *pline="1|2|3|4|5|";
    char *line_tmp=NULL;

    char line_val[10]={0};
    int cnt=0;

    while(1)
    {
                memset(line_val, 0, sizeof(line_val));
                line_tmp=FirStrtok(pline,line_val,"|");


/**
                if(line_tmp == NULL)
**/
                if(!strlen(line_tmp))
                {
                        break;
                }

                printf("val=[%s] \n",line_val);
                printf("line_tmp=[%s] \n",line_tmp);
/*
                pline = NULL; 
*/
                pline = line_tmp;
                line_tmp = NULL;
    }



    return 0 ;
}
