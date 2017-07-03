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
        }else
                strcpy(outstr,instr);
      
        return NULL;
}

main ()
{
    char *line="1|2|3|4|55|66";
    char *tmpline;
    char line_var[10];
    memset(line_var,0,sizeof(line_var));

    for(int i=0;i<9;i++)
   {
    tmpline=NULL ;
    tmpline=FirStrtok(line,line_var,"|");
    line = tmpline;
    printf("i=[%d] line_var=[%s] line=[%s]\n",i,line_var,line);
   }

    return 0 ;
}
