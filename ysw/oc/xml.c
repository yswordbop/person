#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include "xml.h"

void s_readxmline_a(FILE *fp,char *line);
void s_readxmline(FILE *fp,char *line);
void s_initread();
int s_xmline_getattr(char *line,char *a_name,char *a_value,int pos);
int AttrTitleGet(_AttrMap * pAttrMap,char * into,char *out);

#define  LOG_MAX_MSG_LEN  1024
#define  LOG_INFO         0
#define  LOG_ERR          9




char *get_time_now()
{
         int     year, mon, day, hour, min, sec;
         static char sTime[15]={0};

         time_t t;
         struct tm *local_time;


         t = time(0);
         local_time = localtime(&t);
         year = 1900 + local_time->tm_year;
         mon = local_time->tm_mon+1;
         day = local_time->tm_mday;
         hour = local_time->tm_hour;
         min = local_time->tm_min;
         sec = local_time->tm_sec;


         sprintf(sTime,"%04d%02d%02d-%02d%02d%02d",year,mon,day,hour,min,sec);

         return sTime ;
}

int uLog(int level, char *pcFormat, ...)                                    
{                                                                           
    char  pcaDispMsg[LOG_MAX_MSG_LEN];                                 
    memset(pcaDispMsg,0x00,sizeof(pcaDispMsg));  
    va_list  argptr;  
    va_start(argptr,pcFormat);   
    vsnprintf(pcaDispMsg,LOG_MAX_MSG_LEN,pcFormat,argptr);  
    va_end(argptr);  
    printf("[%s]uLog%d[%s]\n",get_time_now(),level,pcaDispMsg); 
    return 0 ; 
}

int main()
{

/**
printf("1111=%s\n",get_time_now());
return 0;
**/

	initCfg_1();
	NetBankMapDeal();

	return 0 ;
}


void s_initread(){
        memset(g_readbuf,'\0',501);
        g_nextreadpos=0;
        g_txcode_in_ind = '0';
        //memset(g_JSON_Buff,0x00,sizeof(g_JSON_Buff));
}

int initCfg_1()
{
        char    sXmlName[512] = {0x00};
	psfp = NULL;

        sprintf(sXmlName, "./xmlmap.xml");
        uLog(LOG_ERR, "配置文件【%s】",sXmlName);
        psfp=fopen(sXmlName,"r");
        if(psfp == NULL){
                uLog(LOG_ERR, "配置文件【%s】打开失败",sXmlName);
                return -1;
        }

	s_initread();
	return 0 ;
}



int NetBankMapDeal(){
	char 	sXmlValue[1025] = {0x00};
	char 	sXmlTmpValue1[1024] = {0x00};
	char 	sXmlTmpValue2[1024] = {0x00};
	char    sXmlName[512] = {0x00};
	char    sXmlLine[1024] = {0x00};
	char	sXmlTmp[1024] = {0x00};
	char	sFormat[256] = {0x00};
	
	char	sTxCode[128] = {0x00};
	char	NEXTTO[128] = {0x00}; 
	char	FLOW[128] = {0x00}; 
	char	DESC[128] = {0x00}; 

	char	sReqName[] = "REQ"; 
	
	
	uLog(LOG_INFO,"%s",__func__);

	while(1){
		uLog(LOG_INFO,"当前的Name = [%s] g_txcode_in_ind = [%c]",AttrMap.name,g_txcode_in_ind);
		ReadAttrMap();
		uLog(LOG_INFO,"当前的Name = [%s] g_txcode_in_ind = [%c]",AttrMap.name,g_txcode_in_ind);
				
		if(strcmp(AttrMap.name,"/MAP") ==0 && g_txcode_in_ind == 'C'){
			g_txcode_in_ind = '0';
			uLog(LOG_INFO,"循环结束");
			uLog(LOG_INFO,"sFormat=【%s】",sFormat);
			break;
		}
		
		//交易代码在配置文件中不存在的情况
		if(strcmp(AttrMap.name,"/MXML") ==0 ){
			uLog(LOG_INFO,"交易代码在配置文件中不存在");
			return -1;
			//goto ErrExit;
			break;
		}
		

		
		if(strcmp(AttrMap.name,"MAP") == 0){
			uLog(LOG_INFO,"进入MAP=【%s】【%s】 但没有进入MAP子域",AttrMap.name,sReadXmlLine);
			//能走到这儿 是进入了MAP了 所以开始检查交易代码
			memset(sXmlTmp,0x00,sizeof(sXmlTmp));
			strcpy(sXmlLine,sReadXmlLine);
			
			pubf_str_rm(sXmlLine,"<");
			pubf_str_rm(sXmlLine,">");
			pubf_str_rm(sXmlLine,"\"");
			
			uLog(LOG_INFO,"sXmlLine 处理特殊字符后【%s】",sXmlLine);
			memset(sReadXmlLine,0x00,sizeof(sReadXmlLine));
			memcpy(sReadXmlLine,sXmlLine + strlen(AttrMap.name) + 1,strlen(sXmlLine) - strlen(AttrMap.name) -1);
			
			uLog(LOG_INFO,"sReadXmlLine 处理特殊字符后【%s】",sReadXmlLine);
			ReadAttrTitleMap();
			AttrTitleGet(&AttrMap,"CODE",sXmlValue);
									
			if(FindStr(sXmlValue,sTxCode)){
				uLog(LOG_INFO,"进入交易代码【%s】【%s】",sXmlValue,sTxCode);
				g_txcode_in_ind = 'C';
				
				AttrTitleGet(&AttrMap,"NEXTTO",NEXTTO);
				AttrTitleGet(&AttrMap,"FLOW",FLOW);
				AttrTitleGet(&AttrMap,"DESC",DESC);
				AttrTitleGet(&AttrMap,"FORMAT",sFormat);
				
				uLog(LOG_INFO,"sFormat=【%s】",sFormat);			
				uLog(LOG_INFO,"NEXTTO=[%s] FLOW=[%s] DESC=[%s]",NEXTTO,FLOW,DESC);
				continue;
			}
			
		}		
		

		if(strcmp(AttrMap.name,"FIRSTWORK") ==0 ){
			uLog(LOG_INFO,"进入FIRSTWORK【%s】",AttrMap.name);
			g_txcode_in_ind = 'F';
			continue;
		}
		if(strcmp(AttrMap.name,"/FIRSTWORK") ==0 && g_txcode_in_ind == 'F'){
			g_txcode_in_ind = '0';
			uLog(LOG_INFO,"退出FIRSTWORK =【%s】",AttrMap.name);
			continue;
		}
		
		if(strcmp(AttrMap.name,"LASTWORK") ==0 ){
			uLog(LOG_INFO,"进入LASTWORK【%s】",AttrMap.name);
			g_txcode_in_ind = 'L';
			continue;
		}
		if(strcmp(AttrMap.name,"/LASTWORK") ==0 && g_txcode_in_ind == 'L'){
			g_txcode_in_ind = '0';
			uLog(LOG_INFO,"退出LASTWORK =【%s】",AttrMap.name);
			continue;
		}
		
		//能走到这儿是交易明细配置
		if(strcmp(sReqName,AttrMap.name) == 0 && (g_txcode_in_ind == 'C' || g_txcode_in_ind == 'F' || g_txcode_in_ind == 'L' )){
			uLog(LOG_INFO,"进入[%c] 子域=【%s】【%s】【%s】",g_txcode_in_ind,AttrMap.name,sReqName,sReadXmlLine);
                        UpdateFileToWork();
			continue;
		}
		AttrMapTitleDestory(&AttrMap);
	}
	//pubf_xml_clr();
	//FDGSXML
	//iVarRemove(".FDGSXML");
	//set_zd_data(".FDGSXML","1");
	AttrMapDestory(&AttrMap);
	
	
}


int ReadAttrMap(){
	char    sXmlName[512] = {0x00};
	char    sXmlLine[1024] = {0x00};
	char	sXmlTmp[1024] = {0x00};
	int	i=0;
	
	_AttrMap *pAttrMap = &AttrMap;
	_AttrTitle *pAttrTitle = NULL;
	
	AttrMapDestory(pAttrMap);
	
	memset(&AttrMap,0x00,sizeof(AttrMap));
	memset(sXmlLine,0x00,sizeof(sXmlLine));
	memset(sReadXmlLine,0x00,sizeof(sReadXmlLine));
	
	s_readxmline(psfp,sReadXmlLine);
	uLog(LOG_INFO,"sReadXmlLine=[%s]",sReadXmlLine);
	
	s_xmline_getname(sReadXmlLine,pAttrMap -> name,0);
	uLog(LOG_INFO,"sValue=[%s]",pAttrMap -> name);
	
	return 0;
}


int ReadAttrTitleMap(){
	char	sXmlTmp[1024] = {0x00};
	int	i=0;
	_AttrMap *pAttrMap = &AttrMap;
	_AttrTitle *pAttrTitle = NULL;
	
	AttrMapTitleDestory(pAttrMap);
	uLog(LOG_INFO,"sReadXmlLine=[%s]",sReadXmlLine);
	while(1){
		memset(sXmlTmp,0x00,sizeof(sXmlTmp));
		pubf_str_idx(sReadXmlLine," ",i,sXmlTmp);
		if(!strlen(sXmlTmp)){
			uLog(LOG_INFO,"遍历结束=[%s]",sXmlTmp);
			break;
		}
		uLog(LOG_INFO," xml[%d]=[%s]",i,sXmlTmp);
		if(i){
			pAttrTitle -> next = (struct _xml_attr_title *)malloc(sizeof(struct _xml_attr_title));	
			memset(pAttrTitle -> next,0x00,sizeof(struct _xml_attr_title));
			pAttrTitle -> next -> parent = pAttrTitle -> parent;
			pAttrTitle = pAttrTitle -> next ;
			
			pubf_str_idx(sXmlTmp,"=",0,pAttrTitle -> key);
			pubf_str_idx(sXmlTmp,"=",1,pAttrTitle -> value);
			//uLog(LOG_INFO,"[%s,%d] %d key=[%s] value = [%s]",__FILE__,__LINE__,i,pAttrTitle -> key ,pAttrTitle -> value);
		}
		else{
			pAttrTitle = (struct _xml_attr_title *)malloc(sizeof(struct _xml_attr_title));
			memset(pAttrTitle,0x00,sizeof(struct _xml_attr_title));
			
			pAttrMap -> next = pAttrTitle;
			pAttrTitle -> parent = pAttrTitle;
		
			pubf_str_idx(sXmlTmp,"=",0,pAttrTitle -> key);
			pubf_str_idx(sXmlTmp,"=",1,pAttrTitle -> value);
			//uLog(LOG_INFO,"[%s,%d] %d key=[%s] value = [%s]",__FILE__,__LINE__,i,pAttrTitle -> key ,pAttrTitle -> value);
		}
		i++;
	}
	//PrintAttrMap(pAttrMap);
	return 0;
}

char *ipubf_str_rm(char *str, char *rm_str)
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



/**
int  nGetField_len(char *name)
{
        long nHashId=0;
        int  i=0;

        TVARNODE *pvar;
        if(name[0]=='\0')
        {
                return -1;
        }
        while(name[i]!='\0'){
                nHashId+=(unsigned char)name[i];
                i++;
        }

        nHashId=nHashId%MAX_DISTINCT_ITEM;
        pvar=(TVARNODE *)&aVars+nHashId;
        while(strcmp(pvar->name,name)!=0){
                pvar=pvar->next;
                if(pvar==NULL){
                        break;
                }
        }
        if(pvar==NULL){
                return(-1);
        }
        return(pvar->length);
}
**/

int AttrMapTitleDestory(_AttrMap * pAttrMap){
        _AttrTitle * pAttrTitle = pAttrMap -> next;
        _AttrTitle * pAttrTitle_temp = pAttrMap -> next;
        uLog(LOG_INFO," pAttrMap addr = [%x]",pAttrMap);
        uLog(LOG_INFO," pAttrMap -> next addr = [%x]",pAttrMap -> next);
        uLog(LOG_INFO," pAttrTitle addr = [%x]",pAttrTitle);
        uLog(LOG_INFO," pAttrTitle_temp addr = [%x]",pAttrTitle_temp);
        while( pAttrTitle ){
                uLog(LOG_INFO," pAttrTitle addr = [%x]",pAttrTitle);
                uLog(LOG_INFO," pAttrTitle -> next addr = [%x]",pAttrTitle -> next);
                uLog(LOG_INFO," pAttrTitle_temp addr = [%x]",pAttrTitle_temp);
                pAttrTitle = pAttrTitle -> next;
                free( pAttrTitle_temp);
                pAttrTitle_temp = pAttrTitle;
        }
        //memset(pAttrMap,0x00,sizeof(_AttrMap));]
        //free(pAttrMap -> next);
        pAttrMap -> next = NULL;
        return 0;
}

int PrintAttrMap(_AttrMap * pAttrMap){
        //if(pAttrMap == NULL) return 0;

        _AttrTitle *pAttrTitle = pAttrMap -> next;
        int i =0;
        uLog(LOG_INFO,"pAttrMap.Name[%s]",pAttrMap -> name);
        while(pAttrTitle){
                uLog(LOG_INFO,"%d key=[%s] value = [%s]",i,pAttrTitle -> key ,pAttrTitle -> value);
                pAttrTitle = pAttrTitle -> next;
                i++;
        }

        return 0;
}

int AttrTitleGet(_AttrMap * pAttrMap,char * into,char *out){
        //if(pAttrMap == NULL) return 0;

        _AttrTitle *pAttrTitle = pAttrMap -> next;
        int i =0;
        uLog(LOG_INFO,"pAttrMap.Name[%s] into = [%s]",pAttrMap -> name,into);
        while(pAttrTitle){
                //uLog(LOG_INFO,"i = %d key=[%s] inkey=[%s] value = [%s]",i,pAttrTitle -> key ,into,pAttrTitle -> value);
                if(0==strcmp(pAttrTitle -> key,into)){
                        memcpy(out,pAttrTitle -> value,strlen(pAttrTitle -> value));
                        break;
                }
                pAttrTitle = pAttrTitle -> next;
                i++;
        }

        return 0;
}

int FindStr(char *from,char *into){
        int i=0;
        while(i <= (strlen(from) - strlen(into))){
                if(0==memcmp(from + i,into,strlen(into))){
                        return 1;
                }
                i++;
        }
        return 0;
}


/************************************************************************
 * 函 数 名：pubf_str_zip_all
 * 函数功能：压缩字符串所有空格
 * 参    数：str(in):被压缩的字符串
 * 返 回 值：压缩后的字符串
************************************************************************/
char *pubf_str_zip_all(char *str)
{
	return pubf_str_rm(str, " ");
}
/************************************************************************
 * 函 数 名：pubf_str_zip_left
 * 函数功能：压缩字符串左侧空格
 * 参    数：str(in):被压缩的字符串
 * 返 回 值：压缩后的字符串
************************************************************************/
char *pubf_str_zip_left(char *str)
{
	char *rm_str = " ";/*需要被压缩掉的字符串*/
	int len = strlen(str);
	int left_space_len = strspn(str, rm_str);
	int new_str_len = len - left_space_len;/*去除左侧空格后长度*/
	for (int i = 0; i < new_str_len && 0 < left_space_len; ++i)
	{
		str[i] = str[left_space_len + i];
	}
	memset(str + new_str_len, '\0', left_space_len);
	return str;
}
/************************************************************************
 * 函 数 名：pubf_str_trim_left
 * 函数功能：压缩字符串左侧trim_str
 * 参    数：str(in):被压缩的字符串
 * 返 回 值：压缩后的字符串
************************************************************************/
char *pubf_str_trim_left(char *str, char *trim_str)
{
	int len = strlen(str);
	int left_space_len = strspn(str, trim_str);
	int new_str_len = len - left_space_len;/*去除左侧空格后长度*/
	for (int i = 0; i < new_str_len && 0 < left_space_len; ++i)
	{
		str[i] = str[left_space_len + i];
	}
	memset(str + new_str_len, '\0', left_space_len);
	return str;
}
/************************************************************************
 * 函 数 名：pubf_str_zip_right
 * 函数功能：压缩字符串右侧空格
 * 参    数：str(in):被压缩的字符串
 * 返 回 值：压缩后的字符串
************************************************************************/
char *pubf_str_zip_right(char *str)
{
	char rm_chr = ' ';/*需要被压缩掉的字符*/
	/*从右向左，遇到' '用'\0'替换，直到非' '字符*/
	for (int i = strlen(str) - 1; rm_chr == str[i]; --i)
	{
		str[i] = '\0';
	}
	return str;
}
/************************************************************************
 * 函 数 名：pubf_str_kp
 * 函数功能：保留str中由kp_str指定的字符
 * 参    数：str(in):被操作的字符串 kp_str:保留的字符
 * 返 回 值：处理后的字符串
************************************************************************/
char *pubf_str_kp(char *str, char *kp_str)
{
	int old_idx = strspn(str, kp_str);
	int new_idx = old_idx;
	int len = strlen(str);
	/*
	old_idx为原字符串索引，new_idx为新字符串索引;
	old_idx从左向右移动，遇kp_str停下;
	否则，old_idx处值存入new_idx处，old_idx和new_idx同步+1
	*/
	for (; old_idx < len; ++new_idx, ++old_idx)
	{
		old_idx += strcspn(str + old_idx, kp_str);
		str[new_idx] = str[old_idx];
	}
	memset(str + new_idx, '\0', len - new_idx);
	return str;
}
/************************************************************************
 * 函 数 名：pubf_str_rm
 * 函数功能：删除str中由rm_str指定的字符
 * 参    数：str(in):被操作的字符串 rm_str:删除的字符
 * 返 回 值：处理后的字符串
************************************************************************/
char *pubf_str_rm(char *str, char *rm_str)
{
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
/************************************************************************
 * 函 数 名：pubf_str_mrg
 * 函数功能：合并str中由mrg_str指定的字符
 * 参    数：str(in):被操作的字符串 mrg_str:合并的字符
 * 返 回 值：处理后的字符串
************************************************************************/
char *pubf_str_mrg(char *str, char *mrg_str)
{
	int len = strlen(str);
	if (1 >= len) 
	{
		return str;
	}
	int old_idx = 0;
	int new_idx = 0;
	int mrg_num = 0;
	/*
	old_idx为原字符串索引，new_idx为新字符串索引;
	old_idx从左向右移动，遇mrg_str跳过;
	否则，old_idx处值存入new_idx处，old_idx + 1处值存入new_idx + 1处，
	old_idx和new_idx同步+2
	*/
	for (; old_idx < len;)
	{
		mrg_num = strspn(str + old_idx, mrg_str);
		str[new_idx++] = str[old_idx];
		if (0== mrg_num) 
		{
			++old_idx;
		}
		else
		{
			old_idx += mrg_num;
		}
	}
	memset(str + new_idx, '\0', len - new_idx);
	return str;
}
/************************************************************************
 * 函 数 名：pubf_str_tolower
 * 函数功能：将字符串中字符转换为小写
 * 参    数：str:被操作的字符串
 * 返 回 值：处理后的字符串
************************************************************************/
char *pubf_str_tolower(char *str)
{
	int i = 0;
	int len = strlen(str);
	for(i=0; i<len; i++)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] = str[i]-'A'+'a';
		}
	}
	return(str);
}

int pubf_str_str2hex(unsigned char *str, unsigned char *hex, int str_len)
{
	for (int i = 0; i < str_len; ++i) 
	{
		sprintf(hex + 2 * i, "%02X", str[i]);
	}
	return 0;
}
int pubf_str_hex2str(unsigned char *hex, unsigned char *str, int str_len)
{
	unsigned char tmp_buf[3] = {0};
	for (int i = 0; i < str_len; ++i) 
	{
		memcpy(tmp_buf, hex + i * 2, 2);
		str[i] = strtol(tmp_buf, NULL, 16);
	}
	return 0;
}

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


int pubf_str_asc2bcd(char *pcASC,char *pcBCD,int iLength)
{
	for(int i=0; i<iLength/2; i++){
		unsigned char ch1,ch2;
		ch1=(unsigned char)pcASC[i*2];
		ch2=(unsigned char)pcASC[i*2+1];
		if(ch1>='a' && ch1<='f')
			ch1=ch1-'a'+0xa;
		else if(ch1>='A' && ch1<='F')
			ch1=ch1-'A'+0xa;
		else
			ch1=ch1-'0';
		if(ch2>='a' && ch2<='f')
			ch2=ch2-'a'+0xa;
		else if(ch2>='A' && ch2<='F')
			ch2=ch2-'A'+0xa;
		else
		ch2=ch2-'0';
		pcBCD[i]=(ch1<<4)|ch2;
	}
	return(0);
}
int pubf_str_bcd2asc(char *pcBCD,char *pcASC,int iLength)
{
	for(int i=0; i<iLength; i++){
		unsigned char ch;
		ch=(unsigned char)pcBCD[i];
		ch=ch>>4;
		if(ch>=10){
			pcASC[2*i]=ch-10+'A';
		}else{
			pcASC[2*i]=ch+'0';
		}
		ch=(unsigned char)pcBCD[i];
		ch=ch &0x0f;
		if(ch>=10){
			pcASC[2*i+1]=ch-10+'A';
		}else{
			pcASC[2*i+1]=ch+'0';
		}
	}
	return(0);
}

int AttrMapDestory(_AttrMap * pAttrMap){
        _AttrTitle * pAttrTitle = pAttrMap -> next;
        _AttrTitle * pAttrTitle_temp = pAttrTitle;
/**
        uLog(LOG_INFO," pAttrMap addr = [%x]",pAttrMap);
        uLog(LOG_INFO," pAttrMap -> next addr = [%x]",pAttrMap -> next);
        uLog(LOG_INFO," pAttrTitle addr = [%x]",pAttrTitle);
        uLog(LOG_INFO," pAttrTitle_temp addr = [%x]",pAttrTitle_temp);
**/
        while( pAttrTitle ){
/**
                uLog(LOG_INFO," pAttrTitle addr = [%x]",pAttrTitle);
                uLog(LOG_INFO," pAttrTitle -> next addr = [%x]",pAttrTitle -> next);
                uLog(LOG_INFO," pAttrTitle_temp addr = [%x]",pAttrTitle_temp);
**/
                pAttrTitle = pAttrTitle -> next;
                free( pAttrTitle_temp);
                pAttrTitle_temp = pAttrTitle;
        }
        //free(pAttrMap -> next);
        //free(pAttrMap);
        memset(pAttrMap,0x00,sizeof(_AttrMap));
        return 0;
}

void s_readxmline(FILE *fp,char *line){/*读 一行*/
        while(1){
                s_readxmline_a(fp,line);
                if(line[0]=='<'){
                        int i=1;
                        while(line[i]!='\0'){
                                if(line[i]==' '){
                                        return;
                                }else if(line[i]=='>'){
                                        return;
                                }else if(line[i]=='/' && line[i+1]=='>'){
                                        break;
                                }else{
                                        i++;
                                }
                        }
                        if(line[i]=='\0'){
                                return;
                        }else{
                                line[0]='\0';
                                continue;
                        }
                }else{
                        return;
                }
        }
        return;
}

/*从一xml行中得到名称部分，并返回名称之后下一个字符的位置*/
int s_xmline_getname(char *line,char *name,int pos){
        int pos1,pos2;

        pos1=pos+1;
        pos1++;

        if(line[pos1]=='/'){
                while(line[pos1]!='>')
                        pos1++;
                memcpy(name,line+pos+1,pos1-pos-1);
                name[pos1-pos-1]='\0';
                return pos1;
        }else{

                while(line[pos1]!=' ' && line[pos1]!='>')
                        pos1++;

                memcpy(name,line+pos+1,pos1-pos-1);
                name[pos1-pos-1]='\0';
                return pos1;
        }

}


void s_readxmline_a(FILE *fp,char *line){/*读 一行*/
	int readlen;
	int buflen;
	int pos1,pos2;
	char ch,ch1,ch2,ch3;
	int binquot,bcomment;
	/*if(g_nextreadpos==sizeof(g_readbuf)-1){
		readlen=fread(g_readbuf,1,sizeof(g_readbuf),fp);
		g_nextreadpos=0;
		g_readbuf[readlen]='\0';
	}*/
	buflen=strlen(g_readbuf);
	uLog(LOG_INFO,"strlen(g_readbuf)=[%d]",buflen);
	/*在读缓冲中读出由一对<>封起来的部分,缓冲区读完要重新读文件*/
	pos1=0;
	line[0]='\0';
	binquot=0;bcomment=0;
	uLog(LOG_INFO,"this is Function:s_readxmline_a()");
	while(1){
		ch=g_readbuf[g_nextreadpos++];
		/*printf("read[%c],readpos=%d\n",ch,g_nextreadpos);*/
		/*getchar();*/
		
		
		
		if(ch=='\0'){
			readlen=fread(g_readbuf,1,sizeof(g_readbuf)-1,fp);
			uLog(LOG_INFO,"readlen=[%d]",readlen);
			g_nextreadpos=0;
			g_readbuf[readlen]='\0';
			if(readlen==0){/*不要最后不完整的行*/
				line[0]='\0';
				break;
			}
			ch=g_readbuf[g_nextreadpos++];
		}
		if(!binquot){/*在双引号外的部分，把所有空白都化作一个空格*/
			if(ch==13 || ch==10 || ch=='\t')
				ch=' ';
			if(ch==' '){/*等号之后也不要空格*/
				if(pos1==0 || line[pos1-1]==' '|| line[pos1-1]=='='|| line[pos1-1]=='<')
					continue;
			}
			if(ch=='"')/*是双引号则改变引号标志*/
				binquot=1;
			if(ch=='=' || ch=='/' )/*若为等号，则删除之前的所有空格*/
				if(line[pos1-1]==' ')
					pos1--;
			if(ch=='-'){/*删除注释*/
				
				ch1=ch2=ch3='\0';
				/*printf("Line=%s\n",line);*/
				if(pos1>2 && line[pos1-1]=='-' && line[pos1-2]=='!' && line[pos1-3]=='<'){
					int bakpos;/*备份之前的pos1位置，当注释走完后恢复*/
					bakpos=pos1-3;
					do{
						
						if(ch2=='-' && ch3=='-'){
							ch1=ch2;
							ch2=ch3;
						}
						else{
							if(ch3=='-'){
								ch1=ch3;
							}else{
							
								ch=g_readbuf[g_nextreadpos++];
								if(ch=='\0'){
									readlen=fread(g_readbuf,1,sizeof(g_readbuf)-1,fp);
									g_nextreadpos=0;
									g_readbuf[readlen]='\0';
									ch=g_readbuf[g_nextreadpos++];
								}
								ch1=ch;
							}
							ch=g_readbuf[g_nextreadpos++];
							if(ch=='\0'){
								readlen=fread(g_readbuf,1,sizeof(g_readbuf)-1,fp);
								g_nextreadpos=0;
								g_readbuf[readlen]='\0';
								ch=g_readbuf[g_nextreadpos++];
							}
							ch2=ch;	
						}
						ch=g_readbuf[g_nextreadpos++];
						if(ch=='\0'){
							readlen=fread(g_readbuf,1,sizeof(g_readbuf)-1,fp);
							g_nextreadpos=0;
							g_readbuf[readlen]='\0';
							ch=g_readbuf[g_nextreadpos++];
						}
						ch3=ch;
					}while(ch1!='-' || ch2!='-' || ch3!='>');
					pos1=bakpos;
					continue;
				}/* end if pos1>1 && line....*/
			}/* end if ch=='-'*/
			if(ch=='?'){/*删除?注释*/
				if(pos1>0 &&   line[pos1-1]=='<'){
					ch1=ch2='\0';
					do{
						if(ch2=='?')
							ch1=ch2;
						else{
							ch=g_readbuf[g_nextreadpos++];
							if(ch=='\0'){
								readlen=fread(g_readbuf,1,sizeof(g_readbuf)-1,fp);
								g_nextreadpos=0;
								g_readbuf[readlen]='\0';
								ch=g_readbuf[g_nextreadpos++];
							}
							ch1=ch;
						}
						ch=g_readbuf[g_nextreadpos++];
						if(ch=='\0'){
							readlen=fread(g_readbuf,1,sizeof(g_readbuf)-1,fp);
							g_nextreadpos=0;
							g_readbuf[readlen]='\0';
							ch=g_readbuf[g_nextreadpos++];
						}
						ch2=ch;	
						
						
					}while(ch1!='?' ||  ch2!='>');
					pos1=0;
					continue;
				}/* end if pos1>0 && line....*/
			}/* end if ch=='?'*/
			/*正常可以写入line的字符*/		
			line[pos1++]=ch;
			if(ch=='>'  ){
				line[pos1]='\0';
				break;
			}
				
		}else{/*else binqot */
			line[pos1++]=ch;
			if(ch=='"')/*是双引号则改变引号标志*/
				binquot=0;
			/*if(ch=='>'){
				line[pos1]='\0';
				break;
			}*/
		}/* end if binquot condition */
	}/* end while */
}


int UpdateFileToWork(){
	
	char 	sXmlValue[1025] = {0x00};
	char 	sXmlTmpValue1[1024] = {0x00};
	char 	sXmlTmpValue2[1024] = {0x00};
	char	sMSGBOX[64] = {0x00};
	pubf_str_rm(sReadXmlLine,"<");
	
	pubf_str_rm(sReadXmlLine,">");
	
	pubf_str_rm(sReadXmlLine,"\"");
	pubf_str_rm(sReadXmlLine,"/");

	//去掉NAME字段
	memset(sXmlValue,0x00,sizeof(sXmlValue));
	memcpy(sXmlValue,sReadXmlLine + strlen(AttrMap.name) +1,strlen(sReadXmlLine) - strlen(AttrMap.name) -1);
	memset(sReadXmlLine,0x00,sizeof(sReadXmlLine));
	memcpy(sReadXmlLine,sXmlValue,strlen(sXmlValue));
	memset(sXmlValue,0x00,sizeof(sXmlValue));
	
	//uLog(LOG_INFO,"sLine=[%s]",sReadXmlLine);
	
	
	ReadAttrTitleMap();
	
	char sTxCode[1024] = {0x00};
	char sFromValue[1024] = {0x00};
	char sToValue[1024] = {0x00};
	char sBeginValue[1024] = {0x00};
	char sLengthValue[1024] = {0x00};
	char sMapBeginValue[1024] = {0x00};
	char sMapLengthValue[1024] = {0x00};
	char sZipValue[1024] = {0x00};
	char sExpValue[1024] = {0x00};
	char sCheck[1024] = {0x00};
	char sCheckGetValue[1024] = {0x00};
	char sCheckValue[1024] = {0x00};
	char sAmtValue[1024] = {0x00};
	char sXmlTitleValue[1024] = {0x00};
	char sUrl[1024]={0x00};
	char sPwd[1024]={0x00};
	char sSpaceValue[1024] = {0x00};
	
	//处理文件明细
	char sFILE[1024] = {0x00};
	
	
	
	AttrTitleGet(&AttrMap,"FROM",sFromValue);
	uLog(LOG_INFO,"FROM=【%s】",sFromValue);
				
	AttrTitleGet(&AttrMap,"TO",sToValue);
	uLog(LOG_INFO,"TO=【%s】",sToValue);
										
	AttrTitleGet(&AttrMap,"BEGIN",sBeginValue);
	uLog(LOG_INFO,"BEGIN=【%s】",sBeginValue);
	
	AttrTitleGet(&AttrMap,"LENGTH",sLengthValue);
	uLog(LOG_INFO,"LENGTH=【%s】",sLengthValue);
	
	//AttrTitleGet(&AttrMap,"APPEND",sAppValue);
	//uLog(LOG_INFO,"APPEND=【%s】",sAppValue);
	
	//AttrTitleGet(&AttrMap,"ZIPSPACE",sZipValue);
	//uLog(LOG_INFO,"ZIPSPACE=【%s】",sZipValue);
	
	AttrTitleGet(&AttrMap,"EXPLAIN",sExpValue);
	uLog(LOG_INFO,"EXPLAIN=【%s】",sExpValue);
	
	AttrTitleGet(&AttrMap,"MAPBEGIN",sMapBeginValue);
	uLog(LOG_INFO,"MAPBEGIN=【%s】",sMapBeginValue);
	
	AttrTitleGet(&AttrMap,"CHECK",sCheck);
	uLog(LOG_INFO,"CHECK FLAG=【%s】",sCheck);
	
	AttrTitleGet(&AttrMap,"VALUE",sCheckValue);
	uLog(LOG_INFO,"CHECK VALUE=【%s】",sCheckValue);
	
	AttrTitleGet(&AttrMap,"AMT",sAmtValue);
	uLog(LOG_INFO,"AMT=【%s】",sAmtValue);
	
	/*20160627 add by wmz*/
	AttrTitleGet(&AttrMap,"SPACE",sSpaceValue);
	uLog(LOG_INFO,"SPACE=【%s】",sSpaceValue);
	
	AttrTitleGet(&AttrMap,"XML",sXmlTitleValue);
	uLog(LOG_INFO,"XML=【%s】",sXmlTitleValue);
	
	AttrTitleGet(&AttrMap,"URL",sUrl);
	uLog(LOG_INFO,"URL=【%s】",sUrl);
	
	AttrTitleGet(&AttrMap,"PWD",sPwd);
	uLog(LOG_INFO,"PWD=【%s】",sPwd);


	strcpy(sTxCode,"gw0018");

	memset(sXmlTmpValue1,0x00,sizeof(sXmlTmpValue1));
	memset(sXmlTmpValue2,0x00,sizeof(sXmlTmpValue2));
	if(!memcmp(sFromValue,"#",1) || !memcmp(sFromValue,".",1)||!memcmp(sFromValue,"$",1)){
		memcpy(sXmlTmpValue1,sFromValue,sizeof(sXmlTmpValue1)-1);
		//uLog(LOG_INFO,"sFromValue=【%s】",sXmlTmpValue1);
	}
	else if(!memcmp(sFromValue,":",1)){
		//get_zd_data("#MSGBOX",sMSGBOX);
		//get_zd_data(sMSGBOX,g_JSON_Buff);
		//nGetJsonVarString(g_JSON_Buff,sFromValue,sXmlTmpValue1);

		memcpy(sXmlTmpValue1,sFromValue,sizeof(sXmlTmpValue1)-1);
/**huang
		pubf_GetVarString(sFromValue,sXmlTmpValue1);
**/
	/*      add at 20170315 宇信发短信会带换行和tab字符，发给短信银行那边前删掉     */
		if(!memcmp(sTxCode,"gw0012",6) && !memcmp(sFromValue,":cd:smsContent",14))
		{
			int icnt=0;
			while(icnt++<10)	/*	因为函数pubf_rmStr只能删除第一次发现的匹配字符串	*/
			{
				pubf_rmStr(sXmlTmpValue1,"\\n\\t\\t");
			}
		}
	}
	else{
		memcpy(sXmlTmpValue1,sFromValue,strlen(sFromValue));	
		//uLog(LOG_INFO,"sFromValue=【%s】",sXmlTmpValue1);
	}

	uLog(LOG_INFO,"FROM=【%s】,TO= 【%s】,VALUE= 【%s】",sFromValue,sToValue,sXmlTmpValue1);
	
	//if( g_txcode_in_ind == 'L' && !memcmp(sFromValue,"#sys13",6) && 0 == strlen(sXmlTmpValue1) )
	
	if(atol(sLengthValue) > 0){
		memcpy(sXmlTmpValue2,sXmlTmpValue1 + atol(sBeginValue),atol(sLengthValue));
	}
	else{
		memcpy(sXmlTmpValue2,sXmlTmpValue1 ,strlen(sXmlTmpValue1));
	}
	
	if(strlen(sFromValue) <= 0){
		memset(sXmlTmpValue2,0x00,sizeof(sXmlTmpValue2));
	}

	
	
	return 0;
}

int pubf_rmStr(char *psSrc, char *psRm)
{
        char *p=strstr(psSrc,psRm);
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
