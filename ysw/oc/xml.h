/**
include list
#include "/home/swyy/include/pubf.h"
#include "/home/swyy/include/ssh_xml.h"
#include "/home/swyy/include/variable.h"
#include "/home/swyy/include/cJSON.h"

**/
struct _xml_attr_map {
        char    name[1024];
        struct  _xml_attr_title * next;
};
struct _xml_attr_title {
        char    key[1024];
        char    value[1024];
        struct  _xml_attr_title * next;
        struct  _xml_attr_title * parent;
};
typedef struct _xml_attr_map _AttrMap;
typedef struct _xml_attr_title _AttrTitle;
_AttrMap AttrMap;
int ReadAttrMap();
FILE    *psfp = NULL;
char g_readbuf[501];/*���ļ�������*/
int  g_nextreadpos;/*��һ�ζ�������λ��*/
/* char    sReadXmlLine[1024] = {0x00}; */
char    sReadXmlLine[2048] = {0x00};
int  g_txcode_in_ind;
/* char g_JSON_Buff[1024]; */
#ifndef __PUBF_H__
#define __PUBF_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errlog.h"
#define SQL_NOTFOUND 100
/**
#define uLog(log_level, ...) usrLog2(log_level, __FILE__, __LINE__, __VA_ARGS__)
**/
/**huang
#ifndef uLog
#define uLog(log_level, ...) telLog(log_level, __FILE__, __LINE__, __VA_ARGS__)
#endif
**/
#define DES_SN 1	/*�������� PLT-����ƽ̨ HSM-���ܻ�  3DES DES����*/
/************************************************************************
 * �� �� ����pubf_3des_enc
 * �������ܣ�����Կkey_str��from_str����3des���ܴ��������into_str�С�
 * ��    ����key_len��key_str���ȣ�from_len��from_str����
 * �� �� ֵ��0 �ɹ� -1ʧ��
 ************************************************************************/
int pubf_3des_enc(unsigned char *key_str, int key_len, unsigned char *from_str, unsigned char *into_str, int from_len);

/************************************************************************
 * �� �� ����pubf_3des_dec
 * �������ܣ�����Կkey_str��from_str����3des���ܴ��������into_str�С�
 * ��    ����key_len��key_str���ȣ�from_len��from_str����
 * �� �� ֵ��0 �ɹ� -1ʧ��
 ************************************************************************/
int pubf_3des_dec(unsigned char *key_str, int key_len, unsigned char *from_str, unsigned char *into_str, int from_len);

/************************************************************************
 * �� �� ����pubf_amt_fd_c2y
 * �������ܣ���cent_fd�е�ֵȡ������"��->Ԫ"�����yuan_fd������123->1.23
 * �� �� ֵ��0 �ɹ�
 ************************************************************************/
int pubf_amt_fd_c2y(char *cent_fd, char *yuan_fd);

/************************************************************************
 * �� �� ����pubf_amt_fd_y2c
 * �������ܣ���yuan_fd�е�ֵȡ������"Ԫ->��"�����cent_fd������1.23->123
 * �� �� ֵ��0 �ɹ�
 ************************************************************************/
int pubf_amt_fd_y2c(char *yuan_fd, char *cent_fd);

/************************************************************************
 * �� �� ���� pubf_cp_fd
 * �������ܣ� ����from�е����ݷ�����to��
 * ��    ���� desc�Ƕ������ݵ�����
 * �� �� ֵ�� 0 �ɹ�; -1 ʧ��
 ************************************************************************/
int pubf_fd_cp(char *from, char *to, char *desc);

/************************************************************************
 * �� �� ���� pubf_cp_fd_zip
 * �������ܣ� ����from�е�����ѹ���������to��
 * ��    ���� desc�Ƕ������ݵ�����
 * �� �� ֵ�� 0 �ɹ�; -1 ʧ��
 ************************************************************************/
int pubf_fd_cp_zip(char *from, char *to, char *desc);

/************************************************************************
 * �� �� ���� pubf_fd_get_zip
 * �������ܣ� ����field�е����ݷ���var��,֮��ѹ��var
 * ��    ���� desc�Ƕ������ݵ�����
 * �� �� ֵ�� 0 �ɹ�
 ************************************************************************/
int pubf_fd_get_zip(char *field, char *var, char *desc);

/************************************************************************
 * �� �� ���� pubf_fd_get
 * �������ܣ� ����field�е����ݷ���var��
 * ��    ���� desc�Ƕ������ݵ�����
 * �� �� ֵ�� 0 �ɹ�
 ************************************************************************/
int pubf_fd_get(char *field, char *var, char *desc);

/************************************************************************
 * �� �� ���� pubf_fd_set
 * �������ܣ� ��var������field��
 * ��    ���� desc�Ƕ������ݵ�����
 * �� �� ֵ�� 0 �ɹ�
 ************************************************************************/
int pubf_fd_set(char *field, char *var, char*desc);

/************************************************************************
 * �� �� ����pubf_fd_list_clr
 * �������ܣ�����fd_list_str����Ӧ��������Ϊ�գ�eg. "#fd1|#fd3|"��
 * �� �� ֵ��0 �ɹ�
 ************************************************************************/
int pubf_fd_list_clr(char *fd_list_str);

/************************************************************************
 * �� �� ���� pubf_fee_get
 * �������ܣ� ��ȡ������
 * ��    ����
 *     ��  ��: �շѱ���fee_code,���׽��amt
 *     ��  ��: ���ֵfee_amt 
 * �� �� ֵ�� 0 �ɹ�; -1 ʧ��
 ************************************************************************/
int pubf_fee_get(char * fee_code,double amt,double *fee_amt);

/************************************************************************
 * �� �� ����pubf_key_load
 * �������ܣ������ն�����term_type���ն˺�term_id����Կ����key_type����Կ
 *           ����key_len�����Կnew_key
 * �� �� ֵ��0 �ɹ� -1 ʧ��
 ************************************************************************/
int pubf_key_load(char *term_type, char *term_id, char *key_type, char *new_key, int key_len);

/************************************************************************
 * �� �� ����pubf_key_save
 * �������ܣ������ն�����term_type���ն˺�term_id����Կ����key_type����Կ
 *           ����key_len������Կnew_key
 * �� �� ֵ��0 �ɹ� -1 ʧ��
 ************************************************************************/
int pubf_key_save(char *term_type, char *term_id, char *key_type, char *new_key, int key_len);

/************************************************************************
 * �� �� ����pubf_key_3des_load
 * �������ܣ������ն�����term_type���ն˺�term_id����Կ����key_type����Կ
 *           ����key_len���������Կ��������Ϊ����new_key
 * �� �� ֵ��0 �ɹ� -1 ʧ��
 ************************************************************************/
int pubf_key_3des_load(char *term_type, char *term_id, char *key_type, char *new_key, int key_len);

/************************************************************************
 * �� �� ����pubf_key_3des_save
 * �������ܣ�����Կnew_key���ܺ󣬸����ն�����term_type���ն˺�term_id��
 *           ��Կ����key_type����Կ����key_len������ܺ����Կ
 * �� �� ֵ��0 �ɹ� -1 ʧ��
 ************************************************************************/
int pubf_key_3des_save(char *term_type, char *term_id, char *key_type, char *new_key, int key_len);

/************************************************************************
 * �� �� ����pubf_map_val2val
 * �������ܣ�����Ŀprj_class��Ӧ��in_valֵӳ��Ϊout_valֵ
 * �� �� ֵ��0 �ɹ�; 1 ʧ��
 ************************************************************************/
int pubf_map_val2val(char *prj_class, char *in_val, char *out_val);

/************************************************************************
 * �� �� ����pubf_map_val2fd
 * �������ܣ�����Ŀprj_class��Ӧ��in_valֵӳ������out_fd���У�descΪ����
 * �� �� ֵ��0 �ɹ�; 1 ʧ��
 ************************************************************************/
int pubf_map_val2fd(char *prj_class, char *in_val, char *out_fd, char *desc);

/************************************************************************
 * �� �� ����pubf_map_fd2val
 * �������ܣ�����Ŀprj_class��Ӧ��in_fd���е�ֵӳ��Ϊout_valֵ��descΪ����
 * �� �� ֵ��0 �ɹ�; 1 ʧ��
 ************************************************************************/
int pubf_map_fd2val(char *prj_class, char *in_fd, char *out_val, char *desc);

/************************************************************************
 * �� �� ����pubf_map_fd2fd
 * �������ܣ�����Ŀprj_class��Ӧ��in_fd�е�ֵӳ��Ϊout_fd�е�ֵ��descΪ����
 * �� �� ֵ��0 �ɹ�; 1 ʧ��
 ************************************************************************/
int pubf_map_fd2fd(char *prj_class, char *in_fd, char *out_fd, char *desc);
 
/************************************************************************
 * �� �� ����pubf_sql_max_long
 * �������ܣ��ӱ�tab_name��fd_name��õ����ֵmax_val(long��)��fmtstrΪ����
 * �� �� ֵ��0 �ɹ�; !0 ʧ��
 ************************************************************************/
int pubf_sql_max_long(char *tab_name, char *fd_name, long *max_val, char *fmtstr, ...);

/************************************************************************
 * �� �� ����pubf_sql_sum
 * �������ܣ��ӱ�tab_name��fd_name��õ���sum_val��fmtstrΪ����
 * �� �� ֵ��0 �ɹ�; !0 ʧ��
 ************************************************************************/
int pubf_sql_sum(char *tab_name, char *fd_name, double *sum_val, char *fmtstr, ...);

/************************************************************************
 * �� �� ����pubf_sql_count
 * �������ܣ��ӱ�tab_name�еõ���������amount��fmtstrΪ����
 * �� �� ֵ��0 �ɹ�; !0 ʧ��
 ************************************************************************/
int pubf_sql_count(char* tab_name, int *amount, char* fmtstr, ... );

/************************************************************************
 * �� �� ����pubf_sql_exec
 * �������ܣ�ִ��sql��䣬fmtstrΪsql�������
 * �� �� ֵ��0 �ɹ�; !0 ʧ��
 ************************************************************************/
int pubf_sql_exec(char* fmtstr, ...);

/************************************************************************
 * �� �� ����pubf_str_kp
 * �������ܣ�����str����kp_strָ�����ַ�
 * ��    ����str(in):���������ַ��� kp_str:�������ַ�
 * �� �� ֵ���������ַ���
************************************************************************/
char *pubf_str_kp(char *str, char *kp_str);

/************************************************************************
 * �� �� ����pubf_str_rm
 * �������ܣ�ɾ��str����rm_strָ�����ַ�
 * ��    ����str(in):���������ַ��� rm_str:ɾ�����ַ�
 * �� �� ֵ���������ַ���
************************************************************************/
char *pubf_str_rm(char *str, char *rm_str);

/************************************************************************
 * �� �� ����pubf_str_mrg
 * �������ܣ��ϲ�str����mrg_strָ�����ַ�
 * ��    ����str(in):���������ַ��� mrg_str:�ϲ����ַ�
 * �� �� ֵ���������ַ���
************************************************************************/
char *pubf_str_mrg(char *str, char *mrg_str);

/************************************************************************
 * �� �� ����pubf_str_zip_all
 * �������ܣ�ѹ���ַ������пո�
 * ��    ����str(in):��ѹ�����ַ���
 * �� �� ֵ��ѹ������ַ���
 ************************************************************************/
char *pubf_str_zip_all(char *str);

/************************************************************************
 * �� �� ����pubf_str_zip_left
 * �������ܣ�ѹ���ַ������ո�
 * ��    ����str(in):��ѹ�����ַ���
 * �� �� ֵ��ѹ������ַ���
 ************************************************************************/
char *pubf_str_zip_left(char *str);

/************************************************************************
 * �� �� ����pubf_str_zip_right
 * �������ܣ�ѹ���ַ����Ҳ�ո�
 * ��    ����str(in):��ѹ�����ַ���
 * �� �� ֵ��ѹ������ַ���
 ************************************************************************/
char *pubf_str_zip_right(char *str);

/************************************************************************
 * �� �� ����pubf_str_trim_left
 * �������ܣ�ѹ���ַ������trim_str
 * ��    ����str(in):��ѹ�����ַ���
 * �� �� ֵ��ѹ������ַ���
************************************************************************/
char *pubf_str_trim_left(char *str, char *trim_str);
char *pubf_str_idx(char *src_str, char *sp_str, int sub_idx, char *sub_str);

/************************************************************************
 * �� �� ����pubf_tx_set_next
 * �������ܣ����ý�����һ����Ϣ
 * ��    ����next_node,��������һ���;next_step,���غ����һ������
 *           next_step_desc,��һ�����������
 * �� �� ֵ��ѹ������ַ���
 * ��    �ӣ�pubf_tx_set_next("accc", "30", "accc����Ӧ��");
 ************************************************************************/
int pubf_tx_set_next(char *next_node, char *next_step, char *next_step_desc);

#define MEMCHECK(FILE, LINE) \
	{\
		char    cmd[256];\
		memset(cmd, 0x0, sizeof(cmd));\
		sprintf(cmd, "memcheck.sh %ld %s %d", getpid(), FILE, LINE);\
		system(cmd);\
	}

#endif
/**********for xml************/
#ifndef __SSH_XML_H__
#define __SSH_XML_H__
#define S_CHILD 0
#define S_BROTHER 1
#define S_ALL_END -1
#define S_NOTALL_END -2

typedef struct  _t_s_xmltree S_XMLTREE;
typedef struct _t_s_xmltnode S_XMLNODE;
struct _t_s_xmltnode{
	struct _t_s_xmltnode *next;
	struct _t_s_xmltnode *firstchild;
	struct _t_s_xmltnode *parent;
	char *name;
	char *value;
	int node_type;/*������ͣ�Ϊģ��oracle 9i��������*/
};
struct _t_s_xmltree{
	struct _t_s_xmltnode *root;
	struct _t_s_xmltnode *current;
};
S_XMLNODE* s_locxmlnode(S_XMLTREE *xml,char *loc);
S_XMLTREE * s_crxmltree(char *filename);
S_XMLTREE * poXmlUnpack(char *pack);
void s_delxmltreenode(S_XMLNODE *pnode);
S_XMLNODE *poXmlAddChild(S_XMLNODE *pnode_cur,char *pcName,char *pcValue);
S_XMLNODE *poXmlAddAttr(S_XMLNODE *,char *,char *);
#define s_delxmltree(pxml){s_delxmltreenode(pxml->root);free( pxml);pxml=NULL;}
#define TEXTVAR S_XMLNODE
TEXTVAR *poGetExtVar(char *name,int creat);
int      nSetExtVarValue(TEXTVAR *pvar,char *value);
int      nSetExtVarAttr(TEXTVAR *pvar,char *attr,char *avalue);
char *   pcGetExtVarValue(TEXTVAR *pvar);
char *   pcGetExtVarAttr(TEXTVAR *pvar,char *attr);
#endif
#ifndef __VARIABLE_H__
#define __VARIABLE_H__
#define MAX_DISTINCT_ITEM 512
#define MAX_VARNAME_LEN 32
typedef struct _t_varnode{
	char name[MAX_VARNAME_LEN];
	char type;	/**a-�ַ�,b-������**/
	char *value;
	short length;	/**���������ݱ�ʾ����,�ַ�������Ӧ��ʵ�ʳ��ȱ���һ��**/
	struct _t_varnode  *next;
}TVARNODE;
extern TVARNODE aVars[MAX_DISTINCT_ITEM];
void vInitVariables();
void vClearVariables();
int  nSetVariable(char *name,char type,char *value,short length);
#define nSetVarString(name,value) nSetVariable(name,'a',value,strlen(value))
#define nSetVarBinary(name,value,length) nSetVariable(name,'b',value,length)
int  nGetVariable(char *name,char *value);	/**���س���,<0��ʾδ�ҵ�***/
int  nSerialize(char *str);		/**���л�,���س���**/
int  nUnSerialize(char *str);	/**����**/
TVARNODE *findItemByName(char *name);
#endif
/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef cJSON__h
#define cJSON__h

#ifdef __cplusplus
extern "C"
{
#endif

/* cJSON Types: */
#define cJSON_False 0
#define cJSON_True 1
#define cJSON_NULL 2
#define cJSON_Number 3
#define cJSON_String 4
#define cJSON_Array 5
#define cJSON_Object 6
	
#define cJSON_IsReference 256

/* The cJSON structure: */
typedef struct cJSON {
	struct cJSON *next,*prev;	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
	struct cJSON *child;		/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

	int type;					/* The type of the item, as above. */

	char *valuestring;			/* The item's string, if type==cJSON_String */
	int valueint;				/* The item's number, if type==cJSON_Number */
	double valuedouble;			/* The item's number, if type==cJSON_Number */

	char *string;				/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
} cJSON;

typedef struct cJSON_Hooks {
      void *(*malloc_fn)(size_t sz);
      void (*free_fn)(void *ptr);
} cJSON_Hooks;

/* Supply malloc, realloc and free functions to cJSON */
extern void cJSON_InitHooks(cJSON_Hooks* hooks);


/* Supply a block of JSON, and this returns a cJSON object you can interrogate. Call cJSON_Delete when finished. */
extern cJSON *cJSON_Parse(const char *value);
/* Render a cJSON entity to text for transfer/storage. Free the char* when finished. */
extern char  *cJSON_Print(cJSON *item);
/* Render a cJSON entity to text for transfer/storage without any formatting. Free the char* when finished. */
extern char  *cJSON_PrintUnformatted(cJSON *item);
/* Delete a cJSON entity and all subentities. */
extern void   cJSON_Delete(cJSON *c);

/* Returns the number of items in an array (or object). */
extern int	  cJSON_GetArraySize(cJSON *array);
/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
extern cJSON *cJSON_GetArrayItem(cJSON *array,int item);
/* Get item "string" from object. Case insensitive. */
extern cJSON *cJSON_GetObjectItem(cJSON *object,const char *string);

/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds. */
extern const char *cJSON_GetErrorPtr(void);
	
/* These calls create a cJSON item of the appropriate type. */
extern cJSON *cJSON_CreateNull(void);
extern cJSON *cJSON_CreateTrue(void);
extern cJSON *cJSON_CreateFalse(void);
extern cJSON *cJSON_CreateBool(int b);
extern cJSON *cJSON_CreateNumber(double num);
extern cJSON *cJSON_CreateString(const char *string);
extern cJSON *cJSON_CreateArray(void);
extern cJSON *cJSON_CreateObject(void);

/* These utilities create an Array of count items. */
extern cJSON *cJSON_CreateIntArray(int *numbers,int count);
extern cJSON *cJSON_CreateFloatArray(float *numbers,int count);
extern cJSON *cJSON_CreateDoubleArray(double *numbers,int count);
extern cJSON *cJSON_CreateStringArray(const char **strings,int count);

/* Append item to the specified array/object. */
extern void cJSON_AddItemToArray(cJSON *array, cJSON *item);
extern void	cJSON_AddItemToObject(cJSON *object,const char *string,cJSON *item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing cJSON to a new cJSON, but don't want to corrupt your existing cJSON. */
extern void cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item);
extern void	cJSON_AddItemReferenceToObject(cJSON *object,const char *string,cJSON *item);

/* Remove/Detatch items from Arrays/Objects. */
extern cJSON *cJSON_DetachItemFromArray(cJSON *array,int which);
extern void   cJSON_DeleteItemFromArray(cJSON *array,int which);
extern cJSON *cJSON_DetachItemFromObject(cJSON *object,const char *string);
extern void   cJSON_DeleteItemFromObject(cJSON *object,const char *string);
	
/* Update array items. */
extern void cJSON_ReplaceItemInArray(cJSON *array,int which,cJSON *newitem);
extern void cJSON_ReplaceItemInObject(cJSON *object,const char *string,cJSON *newitem);

/* Duplicate a cJSON item */
extern cJSON *cJSON_Duplicate(cJSON *item,int recurse);
/* Duplicate will create a new, identical cJSON item to the one you pass, in new memory that will
need to be released. With recurse!=0, it will duplicate any children connected to the item.
The item->next and ->prev pointers are always zero on return from Duplicate. */

/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
extern cJSON *cJSON_ParseWithOpts(const char *value,const char **return_parse_end,int require_null_terminated);

/* Macros for creating things quickly. */
#define cJSON_AddNullToObject(object,name)		cJSON_AddItemToObject(object, name, cJSON_CreateNull())
#define cJSON_AddTrueToObject(object,name)		cJSON_AddItemToObject(object, name, cJSON_CreateTrue())
#define cJSON_AddFalseToObject(object,name)		cJSON_AddItemToObject(object, name, cJSON_CreateFalse())
#define cJSON_AddBoolToObject(object,name,b)	cJSON_AddItemToObject(object, name, cJSON_CreateBool(b))
#define cJSON_AddNumberToObject(object,name,n)	cJSON_AddItemToObject(object, name, cJSON_CreateNumber(n))
#define cJSON_AddStringToObject(object,name,s)	cJSON_AddItemToObject(object, name, cJSON_CreateString(s))

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define cJSON_SetIntValue(object,val)			((object)?(object)->valueint=(object)->valuedouble=(val):(val))

#ifdef __cplusplus
}
#endif

#endif
