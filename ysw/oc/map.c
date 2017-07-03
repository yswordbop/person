#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iconv.h>
#include <libxml/parser.h>
#include <libxml/tree.h>


int main (int argc , char **argv)
{
	char sInbuf[10240]={0x00};
	char sOutbuf[10240]={0x00};
	FILE *fp=NULL;
	char *pMsgBuffGbk = NULL;
	char fname[51] = {0x00};
	char fna[] = "map.xml";
	int pMsgBuffGbk_len=0 ;

        struct stat stBuf;

	xmlDocPtr pdoc = NULL;
	xmlNodePtr proot = NULL, pcur = NULL;

        sprintf(fname,"./%s",fna);

        fp=fopen(fname,"r");
        if(fp==NULL)
        {
           printf("open %s err!\n",fname);
           return -1 ;
        }

        if(stat(fname,&stBuf)<0)
        {
           printf("stat %s err!\n",fname);
           return -1 ;
        }

        fread(sInbuf,stBuf.st_size,1,fp);
        pMsgBuffGbk=(char *)malloc(stBuf.st_size+1024);
        memset(pMsgBuffGbk,0x00,stBuf.st_size+1024);

        //g2u(sInbuf,strlen(sInbuf),pMsgBuffGbk,&pMsgBuffGbk_len);
        g2u1(sInbuf,pMsgBuffGbk);
        printf("sInbuf=[%s]\n",sInbuf);
        printf("pMsgBuffGbk=[%s]\n",pMsgBuffGbk);

        u2g1(pMsgBuffGbk,sInbuf);
        printf("pMsgBuffGbk=[%s]\n",pMsgBuffGbk);
        printf("sInbuf=[%s]\n",sInbuf);

        fclose(fp);

        sprintf(fname,"./%s.utf8",fna);

        fp=fopen(fname,"w+") ;
        if(fp==NULL)
        {
           printf("open %s err!\n",fname);
           return -1 ;
        }
 
     
        fwrite(pMsgBuffGbk,strlen(pMsgBuffGbk),1,fp);
        fclose(fp);
        free(pMsgBuffGbk);

	/*****************��xml�ĵ�********************/
	xmlKeepBlanksDefault(0);//������ϣ���ֹ�����Ԫ��ǰ��Ŀհ��ı����ŵ���һ��node
	pdoc = xmlReadFile (fname, "UTF-8", XML_PARSE_RECOVER);//libxmlֻ�ܽ���UTF-8��ʽ����

	if (pdoc == NULL)
	{
		printf ("error:can't open file!\n");
		exit (1);
	}

	/*****************��ȡxml�ĵ�����ĸ��ڶ���********************/
	proot = xmlDocGetRootElement (pdoc);

	if (proot == NULL)
	{
		printf("error: file is empty!\n");
		exit (1);
	}

	pcur = proot->xmlChildrenNode;

	while (pcur != NULL)
	{
	 	//��ͬ��׼C�е�char����һ����xmlCharҲ�ж�̬�ڴ���䣬�ַ��������� 
                //��غ���������xmlMalloc�Ƕ�̬�����ڴ�ĺ�����xmlFree�����׵��ͷ��ڴ溯����xmlStrcmp���ַ����ȽϺ�����.
		//����char* ch="book", xmlChar* xch=BAD_CAST(ch)����xmlChar* xch=(const xmlChar *)(ch)
		//����xmlChar* xch=BAD_CAST("book")��char* ch=(char *)(xch)
		if (!xmlStrcmp(pcur->name, BAD_CAST("MAP")))
		{
                        /*jx map*/
                        
                   if(!xmlStrcmp(xmlGetProp(pcur, "CODE"),"gw0018"))
                   {
                        printf("into gw0018 attr:NEXTTO->%s DESC->%s \n",xmlGetProp(pcur, "NEXTTO"),xmlGetProp(pcur, "DESC"));

			xmlNodePtr nptr=pcur->xmlChildrenNode;
			while (nptr != NULL)
			{
			       printf("%s->%s\n",nptr->name,((char*)XML_GET_CONTENT(nptr->xmlChildrenNode)));
			       printf("FROM->%s\n",xmlGetProp(nptr, "FROM"));
			       printf("TO->%s\n",xmlGetProp(nptr, "TO"));
			       printf("EXPLAIN->%s\n",xmlGetProp(nptr, "EXPLAIN"));


/**
                              //���Ҵ�������attribute�Ľڵ�
                              if (xmlHasProp(nptr,BAD_CAST "lang"))
                              {

                                  xmlChar* attr_value = NULL;  

                                  attr_value = xmlGetProp(nptr, "lang");  
                                  printf("lang value:%s\n",attr_value);  
                                  xmlFree(attr_value);  

                              }
**/

                              nptr=nptr->next ;
			}
                   }

		}
		pcur = pcur->next;
	}

	/*****************�ͷ���Դ********************/
	xmlFreeDoc (pdoc);
	xmlCleanupParser ();
	xmlMemoryDump ();
	return 0;
}

/************************************************************************
 * ��������: code_conv
 * ��������: ���ڲ�ͬ����֮���ת��
 * ��    ��: from_charsetԴ���� to_charsetĿ�����
 * �� �� ֵ: 0/-1
************************************************************************/
int code_conv(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, int *outlen)
{
	iconv_t cd = iconv_open(to_charset, from_charset);
	if(cd == (iconv_t)(-1))
	{
		return(-1);
	}

	size_t in_left = inlen;
	size_t out_left = inlen * 2;

	if(iconv(cd, &inbuf, &in_left, &outbuf, &out_left) == (size_t)-1)
	{
		iconv_close(cd);
		return(-1);
	}

	*outlen = inlen * 2 - out_left;

	iconv_close(cd);

	return(0);
}
/************************************************************************
 * ��������: code_convert
 * ��������: ���ڲ�ͬ����֮���ת��
 * ��    ��: from_charsetԴ���� to_charsetĿ�����
 * �� �� ֵ: 0/-1
************************************************************************/
int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t *inlen, char *outbuf, size_t *outlen)
{
	iconv_t cd;
	int  rc;
	char **pin  = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if(cd == (iconv_t)(-1))
	{
		return(-1);
	}

	if(iconv(cd, pin, inlen, pout, outlen) == (size_t)-1)
	{
		iconv_close(cd);
		return(-1);
	}
	iconv_close(cd);

	return(0);
}

/************************************************************************
 * ��������: g2u
 * ��������: ��GBK�����ַ���ת��ΪUTF-8����
 * ��    ��: gbk_str GBK�����ַ���  utf_str UTF-8�����ַ���
 * �� �� ֵ: 0/-1
************************************************************************/
int g2u(char *gbk_str, int gbk_len, char *utf_str, int *utf_len)
{
	char *buf = NULL;
	size_t inlen, outlen;

	inlen =  gbk_len;
	outlen = inlen*2 + 1; 
	

	buf = (char *)malloc(outlen+1);
	memset(buf, 0, sizeof(outlen+1));

	if(code_convert("GBK", "UTF-8", gbk_str, &inlen, buf, &outlen))
	{
		free(buf);
		return(-1);
	}

	*utf_len = gbk_len*2 + 1 - outlen;
	memcpy(utf_str, buf, *utf_len);
	utf_str[*utf_len] = '\0';

	free(buf);

	return(0);
}

/************************************************************************
 * ��������: u2g
 * ��������: ��UTF-8����ת��ΪGBK����
 * ��    ��: utf_str UTF-8�����ַ��� gbk_str GBK�����ַ���
 * �� �� ֵ: 0/-1
************************************************************************/
int u2g(char *utf_str, int utf_len, char *gbk_str, int *gbk_len)
{
	char *buf = NULL;
	size_t inlen, outlen;

	inlen  = utf_len;
	outlen = inlen*2 + 1;

	buf = (char *)malloc(outlen+1);
	memset(buf, 0, sizeof(outlen+1));

	if(code_convert("UTF-8", "GBK", utf_str, &inlen, buf, &outlen))
	{
		free(buf);
		return(-1);
	}

	*gbk_len = utf_len*2 + 1 - outlen;
	memcpy(gbk_str, buf, *gbk_len);
	gbk_str[*gbk_len] = '\0';

	free(buf);
 
	return(0);
}

/************************************************************************
 * ��������: g2u1
 * ��������: ��GBK�����ַ���ת��ΪUTF-8����
 * ��    ��: gbk_str GBK�����ַ���  utf_str UTF-8�����ַ���
 * �� �� ֵ: 0/-1
************************************************************************/
int g2u1(char *gbk_str,char *utf_str)
{
	char *buf = NULL;
        int gbk_len,utf_len;
	size_t inlen, outlen;

        gbk_len=strlen(gbk_str);

	inlen =  gbk_len;
	outlen = inlen*2 + 1; 
	

	buf = (char *)malloc(outlen+1);
	memset(buf, 0, sizeof(outlen+1));

	if(code_convert("GBK", "UTF-8", gbk_str, &inlen, buf, &outlen))
	{
		free(buf);
                return -1 ;
	}

	utf_len = gbk_len*2 + 1 - outlen;
	memcpy(utf_str, buf, utf_len);
	utf_str[utf_len] = '\0';

	free(buf);

        return 0 ;
}

/************************************************************************
 * ��������: u2g1
 * ��������: ��UTF-8����ת��ΪGBK����
 * ��    ��: utf_str UTF-8�����ַ��� gbk_str GBK�����ַ���
 * �� �� ֵ: 0/-1
************************************************************************/
int u2g1(char *utf_str,char *gbk_str)
{
	char *buf = NULL;
	size_t inlen, outlen;
        int gbk_len=0 ;

        int utf_len=strlen(utf_str);

	inlen  = utf_len;
	outlen = inlen*2 + 1;

	buf = (char *)malloc(outlen+1);
	memset(buf, 0, sizeof(outlen+1));

	if(code_convert("UTF-8", "GBK", utf_str, &inlen, buf, &outlen))
	{
		free(buf);
		return(-1);
	}

	gbk_len = utf_len*2 + 1 - outlen;
	memcpy(gbk_str, buf, gbk_len);
	gbk_str[gbk_len] = '\0';

	free(buf);
 
	return(0);
}
