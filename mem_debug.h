
#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#include "uthash.h"

enum __dg_flag
{
    __e_flag_malloc,
    __e_flag_calloc,
    __e_flag_free
};

#define MEM_DEBUG_MAX_INFO_LEN     128

#define  MEM_SIZE(type, member)   sizeof(((type*)0)->member)
#define dg_malloc(sz)      __dg_op_alloc_free(sz,1,NULL,__e_flag_malloc,__FILE__,__LINE__,__FUNCTION__)
#define dg_calloc(sz,num)  __dg_op_alloc_free(sz,num,NULL,__e_flag_calloc,__FILE__,__LINE__,__FUNCTION__)
#define dg_free(ptr)       __dg_op_alloc_free(0,0,ptr,__e_flag_free,__FILE__,__LINE__,__FUNCTION__)

#define real_malloc(sz)      malloc(sz)
#define real_calloc(sz,num)  calloc(sz,num)
#define real_free(ptr)       free(ptr)

struct mem_debug_node
{
    UT_hash_handle hh;
    void *ptr;
    unsigned int len;
    char info[MEM_DEBUG_MAX_INFO_LEN];
};
extern void* __dg_op_alloc_free(unsigned int len,unsigned int num,void *ptr,int flag,const char *file,int line,const char *func);
extern int mem_debug_show_all(FILE *fp);
#endif
