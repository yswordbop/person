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
char g_readbuf[501];/*读文件缓冲区*/
int  g_nextreadpos;/*下一次读缓冲区位置*/
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
#define DES_SN 1	/*加密类型 PLT-加密平台 HSM-加密机  3DES DES加密*/
/************************************************************************
 * 函 数 名：pubf_3des_enc
 * 函数功能：用密钥key_str对from_str进行3des加密处理，结果在into_str中。
 * 参    数：key_len是key_str长度；from_len是from_str长度
 * 返 回 值：0 成功 -1失败
 ************************************************************************/
int pubf_3des_enc(unsigned char *key_str, int key_len, unsigned char *from_str, unsigned char *into_str, int from_len);

/************************************************************************
 * 函 数 名：pubf_3des_dec
 * 函数功能：用密钥key_str对from_str进行3des解密处理，结果在into_str中。
 * 参    数：key_len是key_str长度；from_len是from_str长度
 * 返 回 值：0 成功 -1失败
 ************************************************************************/
int pubf_3des_dec(unsigned char *key_str, int key_len, unsigned char *from_str, unsigned char *into_str, int from_len);

/************************************************************************
 * 函 数 名：pubf_amt_fd_c2y
 * 函数功能：将cent_fd中的值取出，由"分->元"后存入yuan_fd，例：123->1.23
 * 返 回 值：0 成功
 ************************************************************************/
int pubf_amt_fd_c2y(char *cent_fd, char *yuan_fd);

/************************************************************************
 * 函 数 名：pubf_amt_fd_y2c
 * 函数功能：将yuan_fd中的值取出，由"元->分"后存入cent_fd，例：1.23->123
 * 返 回 值：0 成功
 ************************************************************************/
int pubf_amt_fd_y2c(char *yuan_fd, char *cent_fd);

/************************************************************************
 * 函 数 名： pubf_cp_fd
 * 函数功能： 将域from中的内容放入域to中
 * 参    数： desc是对域内容的描述
 * 返 回 值： 0 成功; -1 失败
 ************************************************************************/
int pubf_fd_cp(char *from, char *to, char *desc);

/************************************************************************
 * 函 数 名： pubf_cp_fd_zip
 * 函数功能： 将域from中的内容压缩后放入域to中
 * 参    数： desc是对域内容的描述
 * 返 回 值： 0 成功; -1 失败
 ************************************************************************/
int pubf_fd_cp_zip(char *from, char *to, char *desc);

/************************************************************************
 * 函 数 名： pubf_fd_get_zip
 * 函数功能： 将域field中的内容放入var中,之后压缩var
 * 参    数： desc是对域内容的描述
 * 返 回 值： 0 成功
 ************************************************************************/
int pubf_fd_get_zip(char *field, char *var, char *desc);

/************************************************************************
 * 函 数 名： pubf_fd_get
 * 函数功能： 将域field中的内容放入var中
 * 参    数： desc是对域内容的描述
 * 返 回 值： 0 成功
 ************************************************************************/
int pubf_fd_get(char *field, char *var, char *desc);

/************************************************************************
 * 函 数 名： pubf_fd_set
 * 函数功能： 将var放入域field中
 * 参    数： desc是对域内容的描述
 * 返 回 值： 0 成功
 ************************************************************************/
int pubf_fd_set(char *field, char *var, char*desc);

/************************************************************************
 * 函 数 名：pubf_fd_list_clr
 * 函数功能：按照fd_list_str把相应的域设置为空（eg. "#fd1|#fd3|"）
 * 返 回 值：0 成功
 ************************************************************************/
int pubf_fd_list_clr(char *fd_list_str);

/************************************************************************
 * 函 数 名： pubf_fee_get
 * 函数功能： 获取手续费
 * 参    数：
 *     输  入: 收费编码fee_code,交易金额amt
 *     输  出: 输出值fee_amt 
 * 返 回 值： 0 成功; -1 失败
 ************************************************************************/
int pubf_fee_get(char * fee_code,double amt,double *fee_amt);

/************************************************************************
 * 函 数 名：pubf_key_load
 * 函数功能：根据终端类型term_type、终端号term_id、密钥类型key_type、密钥
 *           长度key_len获得密钥new_key
 * 返 回 值：0 成功 -1 失败
 ************************************************************************/
int pubf_key_load(char *term_type, char *term_id, char *key_type, char *new_key, int key_len);

/************************************************************************
 * 函 数 名：pubf_key_save
 * 函数功能：根据终端类型term_type、终端号term_id、密钥类型key_type、密钥
 *           长度key_len保存密钥new_key
 * 返 回 值：0 成功 -1 失败
 ************************************************************************/
int pubf_key_save(char *term_type, char *term_id, char *key_type, char *new_key, int key_len);

/************************************************************************
 * 函 数 名：pubf_key_3des_load
 * 函数功能：根据终端类型term_type、终端号term_id、密钥类型key_type、密钥
 *           长度key_len获得密文密钥，并解密为明文new_key
 * 返 回 值：0 成功 -1 失败
 ************************************************************************/
int pubf_key_3des_load(char *term_type, char *term_id, char *key_type, char *new_key, int key_len);

/************************************************************************
 * 函 数 名：pubf_key_3des_save
 * 函数功能：将密钥new_key加密后，根据终端类型term_type、终端号term_id、
 *           密钥类型key_type、密钥长度key_len保存加密后的密钥
 * 返 回 值：0 成功 -1 失败
 ************************************************************************/
int pubf_key_3des_save(char *term_type, char *term_id, char *key_type, char *new_key, int key_len);

/************************************************************************
 * 函 数 名：pubf_map_val2val
 * 函数功能：将项目prj_class对应的in_val值映射为out_val值
 * 返 回 值：0 成功; 1 失败
 ************************************************************************/
int pubf_map_val2val(char *prj_class, char *in_val, char *out_val);

/************************************************************************
 * 函 数 名：pubf_map_val2fd
 * 函数功能：将项目prj_class对应的in_val值映射后存入out_fd域中，desc为描述
 * 返 回 值：0 成功; 1 失败
 ************************************************************************/
int pubf_map_val2fd(char *prj_class, char *in_val, char *out_fd, char *desc);

/************************************************************************
 * 函 数 名：pubf_map_fd2val
 * 函数功能：将项目prj_class对应的in_fd域中的值映射为out_val值，desc为描述
 * 返 回 值：0 成功; 1 失败
 ************************************************************************/
int pubf_map_fd2val(char *prj_class, char *in_fd, char *out_val, char *desc);

/************************************************************************
 * 函 数 名：pubf_map_fd2fd
 * 函数功能：将项目prj_class对应的in_fd中的值映射为out_fd中的值，desc为描述
 * 返 回 值：0 成功; 1 失败
 ************************************************************************/
int pubf_map_fd2fd(char *prj_class, char *in_fd, char *out_fd, char *desc);
 
/************************************************************************
 * 函 数 名：pubf_sql_max_long
 * 函数功能：从表tab_name的fd_name域得到最大值max_val(long型)，fmtstr为条件
 * 返 回 值：0 成功; !0 失败
 ************************************************************************/
int pubf_sql_max_long(char *tab_name, char *fd_name, long *max_val, char *fmtstr, ...);

/************************************************************************
 * 函 数 名：pubf_sql_sum
 * 函数功能：从表tab_name的fd_name域得到和sum_val，fmtstr为条件
 * 返 回 值：0 成功; !0 失败
 ************************************************************************/
int pubf_sql_sum(char *tab_name, char *fd_name, double *sum_val, char *fmtstr, ...);

/************************************************************************
 * 函 数 名：pubf_sql_count
 * 函数功能：从表tab_name中得到数据总数amount，fmtstr为条件
 * 返 回 值：0 成功; !0 失败
 ************************************************************************/
int pubf_sql_count(char* tab_name, int *amount, char* fmtstr, ... );

/************************************************************************
 * 函 数 名：pubf_sql_exec
 * 函数功能：执行sql语句，fmtstr为sql语句内容
 * 返 回 值：0 成功; !0 失败
 ************************************************************************/
int pubf_sql_exec(char* fmtstr, ...);

/************************************************************************
 * 函 数 名：pubf_str_kp
 * 函数功能：保留str中由kp_str指定的字符
 * 参    数：str(in):被操作的字符串 kp_str:保留的字符
 * 返 回 值：处理后的字符串
************************************************************************/
char *pubf_str_kp(char *str, char *kp_str);

/************************************************************************
 * 函 数 名：pubf_str_rm
 * 函数功能：删除str中由rm_str指定的字符
 * 参    数：str(in):被操作的字符串 rm_str:删除的字符
 * 返 回 值：处理后的字符串
************************************************************************/
char *pubf_str_rm(char *str, char *rm_str);

/************************************************************************
 * 函 数 名：pubf_str_mrg
 * 函数功能：合并str中由mrg_str指定的字符
 * 参    数：str(in):被操作的字符串 mrg_str:合并的字符
 * 返 回 值：处理后的字符串
************************************************************************/
char *pubf_str_mrg(char *str, char *mrg_str);

/************************************************************************
 * 函 数 名：pubf_str_zip_all
 * 函数功能：压缩字符串所有空格
 * 参    数：str(in):被压缩的字符串
 * 返 回 值：压缩后的字符串
 ************************************************************************/
char *pubf_str_zip_all(char *str);

/************************************************************************
 * 函 数 名：pubf_str_zip_left
 * 函数功能：压缩字符串左侧空格
 * 参    数：str(in):被压缩的字符串
 * 返 回 值：压缩后的字符串
 ************************************************************************/
char *pubf_str_zip_left(char *str);

/************************************************************************
 * 函 数 名：pubf_str_zip_right
 * 函数功能：压缩字符串右侧空格
 * 参    数：str(in):被压缩的字符串
 * 返 回 值：压缩后的字符串
 ************************************************************************/
char *pubf_str_zip_right(char *str);

/************************************************************************
 * 函 数 名：pubf_str_trim_left
 * 函数功能：压缩字符串左侧trim_str
 * 参    数：str(in):被压缩的字符串
 * 返 回 值：压缩后的字符串
************************************************************************/
char *pubf_str_trim_left(char *str, char *trim_str);
char *pubf_str_idx(char *src_str, char *sp_str, int sub_idx, char *sub_str);

/************************************************************************
 * 函 数 名：pubf_tx_set_next
 * 函数功能：设置交易下一步信息
 * 参    数：next_node,发往的下一结点;next_step,返回后的下一处理步骤
 *           next_step_desc,下一处理步骤的描述
 * 返 回 值：压缩后的字符串
 * 例    子：pubf_tx_set_next("accc", "30", "accc记账应答");
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
	int node_type;/*结点类型，为模拟oracle 9i解析器用*/
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
	char type;	/**a-字符,b-二进制**/
	char *value;
	short length;	/**二进制数据表示长度,字符型数据应与实际长度保持一致**/
	struct _t_varnode  *next;
}TVARNODE;
extern TVARNODE aVars[MAX_DISTINCT_ITEM];
void vInitVariables();
void vClearVariables();
int  nSetVariable(char *name,char type,char *value,short length);
#define nSetVarString(name,value) nSetVariable(name,'a',value,strlen(value))
#define nSetVarBinary(name,value,length) nSetVariable(name,'b',value,length)
int  nGetVariable(char *name,char *value);	/**返回长度,<0表示未找到***/
int  nSerialize(char *str);		/**序列化,返回长度**/
int  nUnSerialize(char *str);	/**解析**/
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
